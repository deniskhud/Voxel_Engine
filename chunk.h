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

const short CHUNK_X = 255;
const short CHUNK_Y = 255;
const short CHUNK_Z = 255;


class Chunk {
public:
	Chunk();
	~Chunk();

	void draw(Shader& shader, glm::vec3 worldLocation);



private:
	GLuint VBO, blockVAO, blockVBO;
	std::vector<glm::vec4> translations;

	std::array<std::array<std::array<std::uint8_t, CHUNK_X>, CHUNK_Y>, CHUNK_Z> chunkData;

	void setupVisibleBlocks();
};



#endif //CHUNK_H
