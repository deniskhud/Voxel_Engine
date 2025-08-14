#include "chunk.h"
#include "frustum.h"
#include "threadPool.h"
extern ThreadPool pool; // глобальный пул потоков
std::mutex readyMutex;
std::queue<ChunkMeshData> readyMeshes;
const int faceOffsets[6][3] = {
    {  0,  0, -1 }, // back
    {  0,  0,  1 }, // front
    { -1,  0,  0 }, // left
    {  1,  0,  0 }, // right
    {  0, -1,  0 }, // bottom
    {  0,  1,  0 }  // top
};

inline FastNoiseLite noise;

static void setupNoiseOnce() {
    static bool initialized = false;
    if (!initialized) {
        noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
        noise.SetFrequency(0.01f);
        noise.SetSeed(1322);
        noise.SetFractalType(FastNoiseLite::FractalType_FBm);
        noise.SetFractalOctaves(4);
        initialized = true;
    }
}

Chunk::Chunk(int chunkX, int chunkZ)
    : chunkCoordX(chunkX), chunkCoordZ(chunkZ)
{
    setupNoiseOnce();

    blockVAO = 0;
    blockVBO = 0;

    // Отправляем задачу в thread pool
    pool.enqueue([this]() {
        this->generateAsync();
    });
}

Chunk::~Chunk() {
    if (blockVBO) glDeleteBuffers(1, &blockVBO);
    if (blockVAO) glDeleteVertexArrays(1, &blockVAO);
}

void Chunk::generateAsync() {
    setChunkData();

    std::vector<vertex> localMesh;
    buildMesh(localMesh);

    {
        std::lock_guard<std::mutex> lock(readyMutex);
        readyMeshes.push({ std::move(localMesh), chunkCoordX, chunkCoordZ });
    }
}

void Chunk::setChunkData() {
    for (int x = 0; x < CHUNK_X; x++) {
        for (int z = 0; z < CHUNK_Z; z++) {
            int worldX = chunkCoordX * CHUNK_X + x;
            int worldZ = chunkCoordZ * CHUNK_Z + z;

            float noiseVal = noise.GetNoise((float)worldX, (float)worldZ);
            int height = static_cast<int>(((noiseVal + 1.0f) * 0.5f) * CHUNK_Y);

            for (int y = 0; y < CHUNK_Y; ++y) {
                if (y > height) {
                    if (y <= 64) chunkData[x][y][z] = 4;
                    else chunkData[x][y][z] = 0; // air
                }
                else if (y == height) chunkData[x][y][z] = 1; // grass
                else if (y > height - 4) chunkData[x][y][z] = 2; // dirt
                else chunkData[x][y][z] = 3; // stone
            }
        }
    }
}

void Chunk::buildMesh(std::vector<vertex>& outMesh) const {
    outMesh.clear();
    for (int x = 0; x < CHUNK_X; ++x) {
        for (int y = 0; y < CHUNK_Y; ++y) {
            for (int z = 0; z < CHUNK_Z; ++z) {
                uint8_t blockID = chunkData[x][y][z];
                if (blockID == 0) continue;

                for (int face = 0; face < 6; ++face) {
                    int nx = x + faceOffsets[face][0];
                    int ny = y + faceOffsets[face][1];
                    int nz = z + faceOffsets[face][2];

                    bool neighborTransparent = true;
                    if (nx >= 0 && nx < CHUNK_X &&
                        ny >= 0 && ny < CHUNK_Y &&
                        nz >= 0 && nz < CHUNK_Z)
                    {
                        neighborTransparent = Block::isColorless(chunkData[nx][ny][nz]);
                    }

                    if (neighborTransparent) {
                        const int faceStart = face * 6 * 8;
                        for (int v = 0; v < 6; ++v) {
                            int vertexStart = faceStart + v * 8;

                            vertex vert;
                            vert.pos = glm::vec3(
                                Block::vertices[vertexStart + 0] + x,
                                Block::vertices[vertexStart + 1] + y,
                                Block::vertices[vertexStart + 2] + z
                            );
                            vert.uv = glm::vec2(
                                Block::vertices[vertexStart + 3],
                                Block::vertices[vertexStart + 4]
                            );
                            vert.normal = glm::vec3(
                                Block::vertices[vertexStart + 5],
                                Block::vertices[vertexStart + 6],
                                Block::vertices[vertexStart + 7]
                            );
                            vert.id = chunkData[x][y][z];
                            outMesh.push_back(vert);
                        }
                    }
                }
            }
        }
    }
}

void Chunk::uploadMeshToGPU(const std::vector<vertex>& verts) {
    meshVertices = verts;

    if (blockVAO == 0) {
        glGenVertexArrays(1, &blockVAO);
        glGenBuffers(1, &blockVBO);
    }

    glBindVertexArray(blockVAO);
    glBindBuffer(GL_ARRAY_BUFFER, blockVBO);
    glBufferData(GL_ARRAY_BUFFER, meshVertices.size() * sizeof(vertex), meshVertices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, pos));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, uv));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, normal));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 1, GL_SHORT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, id));

    glBindVertexArray(0);
}

void Chunk::draw(Shader& shader, glm::vec3 worldLocation, Camera& camera, int& rc) {
    Frustum frustum = createFrustumFromCamera(
        camera,
        800.0f / 600.0f,
        glm::radians(90.0f),
        0.1f,
        2000.f
    );

    glm::vec3 min = worldLocation;
    glm::vec3 max = worldLocation + glm::vec3(CHUNK_X, CHUNK_Y, CHUNK_Z);

    if (!frustum.isBoxVisible(min, max)) return;

    shader.use();
    shader.setMat4("model", glm::translate(glm::mat4(1.0f), worldLocation));

    glBindVertexArray(blockVAO);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(meshVertices.size()));
    rc++;
}

