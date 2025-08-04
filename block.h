#ifndef BLOCK_H
#define BLOCK_H

enum class Direction {
	Front, Back, Left, Right, Top, Bottom
};


#include "glad/include/glad/glad.h"
#include "glm-1.0.1/glm/glm.hpp"
#include "stb_image.h"
#include <iostream>
#include "shader.h"
#include <filesystem>
class Block {
public:
	static const float* getFaceVertices(Direction dir);

	static float* vertices;
	//x , y, z, u, v, nx, ny, nz = 8; 6 edges * 2 triangles * 3 vertex = 36; -> 8 * 36 = 288
	static constexpr int verticesSize = 288;
	static constexpr float texSize = 1.f;

	Block();

	~Block();
	//initialize a block
	static void init();
	//draw a block
	static void draw();
	//clean
	static void destroy();

	static bool isColorless(std::uint8_t blockID);


private:




	GLuint VAO, VBO, EBO;
	//textures
	GLuint textureSide = 0, textureBot = 0, textureTop = 0;

};







#endif //BLOCK_H