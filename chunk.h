#ifndef CHUNK_H
#define CHUNK_H

#include <vector>
#include <array>
#include <iostream>
#include "glm-1.0.1/glm/glm.hpp"
#include "glm-1.0.1/glm/gtc/matrix_transform.hpp"
#include "glm-1.0.1/glm/gtc/type_ptr.hpp"
#include "glad/include/glad/glad.h"
#include <GLFW/glfw3.h>
#include "block.h"
#include "shader.h"
#include "fastNoiseLite.h"

const short CHUNK_X = 16;
const short CHUNK_Y = 255;
const short CHUNK_Z = 16;


class Chunk {
public:
	Chunk(int chunkX, int chunkZ);
	~Chunk();

	void draw(Shader& shader, glm::vec3 worldLocation);



private:
	GLuint VBO, blockVAO, blockVBO;
	std::vector<glm::vec4> translations;

	std::array<std::array<std::array<std::uint8_t, CHUNK_Z>, CHUNK_Y>, CHUNK_X> chunkData;

	int chunkCoordX = 0, chunkCoordZ = 0;

	void setupVisibleBlocks();
};



#endif //CHUNK_H