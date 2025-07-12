#ifndef CHUNK_H
#define CHUNK_H

#include <vector>
#include <iostream>
#include "glm-1.0.1/glm/glm.hpp"
#include "glm-1.0.1/glm/gtc/matrix_transform.hpp"
#include "glm-1.0.1/glm/gtc/type_ptr.hpp"
#include "glad/include/glad/glad.h"
#include <GLFW/glfw3.h>

#include "stb_image.h"


const short CHUNK_X = 16;
const short CHUNK_Y = 16;
const short CHUNK_Z = 16;

enum class BLOCK_TYPE {
	AIR, DIRT, GRASS, STONE
};

struct BLOCK {
	BLOCK_TYPE type;
};

class Chunk {
public:
	/*** getters / setters ***/
	void setBlock(BLOCK_TYPE type, int x, int y, int z);
	BLOCK_TYPE getBlock(int x, int y, int z);

	glm::ivec3 coord = glm::ivec3(0); // или передавать в конструктор

	void generateTypes();
	void generateMesh();

	void draw();
	void destroyChunk();

private:
	BLOCK blocks[CHUNK_X][CHUNK_Y][CHUNK_Z];
	std::vector<float> verts;

	unsigned int vao = 0;
	unsigned int vbo = 0;


	void appendFace(std::vector<float>& v,
					   int face,                      // 0..5
					   const glm::vec3& blockWorldPos,// положение блока
					   float tileU0, float tileV0,    // левый‑нижний угол тайла в атласе
					   float tileSize);

	// templates for draw and vertex displacement
	static const glm::vec3 FACE_POS[6][4];

	static const glm::vec2 FACE_UV[4];
	static const glm::ivec3 NEIGHBOUR_OFF[6];


};

/*** getters / setters ***/
inline BLOCK_TYPE Chunk::getBlock(int x, int y, int z) {
	if (x < 0 || x > CHUNK_X && y < 0 || y > CHUNK_Y && z < 0 || z > CHUNK_Z) {
		return BLOCK_TYPE::AIR;
	}
	return blocks[x][y][z].type;
}
inline void Chunk::setBlock(BLOCK_TYPE type, int x, int y, int z) {
	if (x < 0 || x > CHUNK_X && y < 0 || y > CHUNK_Y && z < 0 || z > CHUNK_Z) {
		return;
	}
	//set a type of block
	blocks[x][y][z].type = type;
	generateMesh();

}



void Chunk::generateTypes() {
	for (int x = 0; x < CHUNK_X; x++) {
		for (int y = 0; y < CHUNK_Y; y++) {
			for (int z = 0; z < CHUNK_Z; z++) {
				blocks[x][y][z].type = BLOCK_TYPE::DIRT;
			}
		}
	}
}

inline void Chunk::generateMesh() {
	verts.clear();

	glm::vec3 chunkOriginWorld = glm::vec3(coord.x * CHUNK_X, coord.y * CHUNK_Y, coord.z * CHUNK_Z);

	constexpr float TILE = 1.0f / 16.0f;

	for (int x = 0; x < CHUNK_X; x++) {
		for (int y = 0; y < CHUNK_Y; ++y) {
			for (int z = 0; z < CHUNK_Z; ++z) {

				BLOCK_TYPE type = blocks[x][y][z].type;
				if (BLOCK_TYPE::AIR == type) continue;


				glm::vec2 tileSide(1 * TILE, 0 * TILE);
				glm::vec2 tileTop(0 * TILE, 0 * TILE);
				glm::vec2 tileBot(2 * TILE, 0 * TILE);

				for (int face = 0; face < 6; ++face) {
					int nx = x + NEIGHBOUR_OFF[face].x;
					int ny = y + NEIGHBOUR_OFF[face].y;
					int nz = z + NEIGHBOUR_OFF[face].z;

					bool neighborSolid =
				   (nx>=0 && nx<CHUNK_X &&
					ny>=0 && ny<CHUNK_Y &&
					nz>=0 && nz<CHUNK_Z) ?
				   blocks[nx][ny][nz].type != BLOCK_TYPE::AIR
				   : false; // за границей считаем воздух

					if (neighborSolid) continue; // грань не видна

					glm::vec2 tile = tileSide;
					if (face == 2) tile = tileTop;   // +Y
					if (face == 3) tile = tileBot;   // -Y

					appendFace(verts, face,
							   chunkOriginWorld + glm::vec3(x,y,z),
							   tile.x, tile.y, TILE);
				}
			}
		}
		//load to GPU
		if (!vao) glGenVertexArrays(1,&vao);
		if (!vbo) glGenBuffers(1,&vbo);

		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER,
					 verts.size()*sizeof(float),
					 verts.data(),
					 GL_STATIC_DRAW);

		glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5*sizeof(float),(void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,5*sizeof(float),(void*)(3*sizeof(float)));
		glEnableVertexAttribArray(1);
	}
}

inline void Chunk::appendFace(std::vector<float> &v, int face, const glm::vec3 &blockWorldPos, float tileU0, float tileV0, float tileSize) {
	// 2 треугольника: 0‑1‑2 и 2‑3‑0
	const int idx[6] = {0,1,2, 2,3,0};

	for (int i=0;i<6;++i) {
		int vi = idx[i];
		glm::vec3 pos = blockWorldPos + FACE_POS[face][vi];
		glm::vec2 uv  = FACE_UV[vi]*tileSize + glm::vec2(tileU0, tileV0);

		v.push_back(pos.x);
		v.push_back(pos.y);
		v.push_back(pos.z);
		v.push_back(uv.x);
		v.push_back(uv.y);
	}
}

inline void Chunk::draw() {
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, verts.size() / 5);
}




#endif //CHUNK_H
