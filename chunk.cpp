#include "chunk.h"


inline FastNoiseLite noise;

static void setupNoise() {
	noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
	noise.SetFrequency(0.01f);     // Настройка "размаха" рельефа
	noise.SetSeed(1322);
	noise.SetFractalType(FastNoiseLite::FractalType_FBm);
	noise.SetFractalOctaves(4);
}

Chunk::Chunk(int chunkX, int chunkZ) : chunkCoordX(chunkX), chunkCoordZ(chunkZ) {
	setupNoise();
	for (int x = 0; x < CHUNK_X; x++) {
		for (int z = 0; z < CHUNK_Z; z++) {
			int worldX = chunkCoordX * CHUNK_X + x;
			int worldZ = chunkCoordZ * CHUNK_Z + z;

			float noiseVal = noise.GetNoise((float)worldX, (float)worldZ);
			int height = static_cast<int>(((noiseVal + 1.0f) * 0.5f) * CHUNK_Y);

			for (int y = 0; y < CHUNK_Y; ++y) {
				if (y > height) {
					chunkData[x][y][z] = 0; // air
				}
				else if (y == height) {
					chunkData[x][y][z] = 1; // grass
				}
				else if (y > height - 4) {
					chunkData[x][y][z] = 2; // dirt
				}
				else {
					chunkData[x][y][z] = 3; // stone
				}
			}
		}
	}

	setupVisibleBlocks();
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * translations.size(), translations.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenVertexArrays(1, &blockVAO);
	glGenBuffers(1, &blockVBO);

	glBindVertexArray(blockVAO);
	glBindBuffer(GL_ARRAY_BUFFER, blockVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * Block::verticesSize, Block::vertices, GL_STATIC_DRAW);

	//position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);

	//uv
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));

	//normals
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);
	glVertexAttribDivisor(3, 1);  // <-- очень важно

	glBindVertexArray(0);

	Block::draw();
}

Chunk::~Chunk() {
	glDeleteBuffers(1, &blockVBO);
	glDeleteVertexArrays(1, &blockVAO);
}

void Chunk::setupVisibleBlocks() {
	bool flag = true;
	for (int x = 0; x < CHUNK_X; ++x) {
		for (int y = 0; y < CHUNK_Y; ++y) {
			for (int z = 0; z < CHUNK_Z; ++z) {
				if (chunkData[x][y][z] == 0) {
					flag = false;
				}
				else if (x == 0 || x == 15 || y == 0 || y == 31 || z == 0 || z == 15) {
					flag = true;
				}
				else {
					flag = ((Block::isColorless(chunkData[x - 1][y][z])) ||
							(Block::isColorless(chunkData[x + 1][y][z])) ||
							(Block::isColorless(chunkData[x][y - 1][z])) ||
							(Block::isColorless(chunkData[x][y + 1][z])) ||
							(Block::isColorless(chunkData[x][y][z - 1])) ||
							(Block::isColorless(chunkData[x][y][z + 1])));
				}
				if (flag) {
					translations.push_back(glm::vec4(x, y, z, chunkData[x][y][z]));
				}
			}
		}
	}
}


void Chunk::draw(Shader &shader, glm::vec3 worldLocation) {
	glBindVertexArray(blockVAO);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, worldLocation);

	shader.setMat4("model", model);
	glDrawArraysInstanced(GL_TRIANGLES, 0,36,  translations.size());
}