#ifndef BLOCK_H
#define BLOCK_H

#include "glad/include/glad/glad.h"
#include "glm-1.0.1/glm/glm.hpp"
#include "stb_image.h"
#include <iostream>
#include "shader.h"
#include <filesystem>
class Block {
public:

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

	void setTextureSide(const std::string& path, Shader& ourShader);
	void setTextureBot(const std::string& path, Shader& ourShader);
	void setTextureTop(const std::string& path, Shader& ourShader);
	void bindTexture(Shader& shader);
private:




	GLuint VAO, VBO, EBO;
	//textures
	GLuint textureSide = 0, textureBot = 0, textureTop = 0;

};







#endif //BLOCK_H
