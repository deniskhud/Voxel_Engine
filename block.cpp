#include "block.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
float* Block::vertices = nullptr;

//initialize a block
void Block::init() {
	 vertices = new float[verticesSize] {
	 	//   position             uv                        the normals(for future)
	    // back
	    -0.5f,  0.5f, -0.5f,  texSize*1.f, texSize*0.f,  0.f, 0.f, -1.f,
	    -0.5f, -0.5f, -0.5f,  texSize*1.f, texSize*1.f,  0.f, 0.f, -1.f,
	     0.5f, -0.5f, -0.5f,  texSize*0.f, texSize*1.f,  0.f, 0.f, -1.f,

	     0.5f, -0.5f, -0.5f,  texSize*0.f, texSize*1.f,  0.f, 0.f, -1.f,
	     0.5f,  0.5f, -0.5f,  texSize*0.f, texSize*0.f,  0.f, 0.f, -1.f,
	    -0.5f,  0.5f, -0.5f,  texSize*1.f, texSize*0.f,  0.f, 0.f, -1.f,

	    // front
	    -0.5f, -0.5f,  0.5f,  texSize*1.f, texSize*1.f,  0.f, 0.f, 1.f,
	    -0.5f,  0.5f,  0.5f,  texSize*1.f, texSize*0.f,  0.f, 0.f, 1.f,
	     0.5f,  0.5f,  0.5f,  texSize*0.f, texSize*0.f,  0.f, 0.f, 1.f,

	     0.5f,  0.5f,  0.5f,  texSize*0.f, texSize*0.f,  0.f, 0.f, 1.f,
	     0.5f, -0.5f,  0.5f,  texSize*0.f, texSize*1.f,  0.f, 0.f, 1.f,
	    -0.5f, -0.5f,  0.5f,  texSize*1.f, texSize*1.f,  0.f, 0.f, 1.f,

	    // left
	    -0.5f,  0.5f, -0.5f,  texSize*0.f, texSize*0.f,  -1.f, 0.f, 0.f,
	    -0.5f,  0.5f,  0.5f,  texSize*1.f, texSize*0.f,  -1.f, 0.f, 0.f,
	    -0.5f, -0.5f,  0.5f,  texSize*1.f, texSize*1.f,  -1.f, 0.f, 0.f,

	    -0.5f, -0.5f,  0.5f,  texSize*1.f, texSize*1.f,  -1.f, 0.f, 0.f,
	    -0.5f, -0.5f, -0.5f,  texSize*0.f, texSize*1.f,  -1.f, 0.f, 0.f,
	    -0.5f,  0.5f, -0.5f,  texSize*0.f, texSize*0.f,  -1.f, 0.f, 0.f,

	    // right
	     0.5f,  0.5f,  0.5f,  texSize*0.f, texSize*0.f,  1.f, 0.f, 0.f,
	     0.5f,  0.5f, -0.5f,  texSize*1.f, texSize*0.f,  1.f, 0.f, 0.f,
	     0.5f, -0.5f, -0.5f,  texSize*1.f, texSize*1.f,  1.f, 0.f, 0.f,

	     0.5f, -0.5f, -0.5f,  texSize*1.f, texSize*1.f,  1.f, 0.f, 0.f,
	     0.5f, -0.5f,  0.5f,  texSize*0.f, texSize*1.f,  1.f, 0.f, 0.f,
	     0.5f,  0.5f,  0.5f,  texSize*0.f, texSize*0.f,  1.f, 0.f, 0.f,

	    // bottom
	    -0.5f, -0.5f, -0.5f,  texSize*0.f, texSize*1.f,  0.f, -1.f, 0.f,
	    -0.5f, -0.5f,  0.5f,  texSize*0.f, texSize*0.f,  0.f, -1.f, 0.f,
	     0.5f, -0.5f,  0.5f,  texSize*1.f, texSize*0.f,  0.f, -1.f, 0.f,

	     0.5f, -0.5f,  0.5f,  texSize*1.f, texSize*0.f,  0.f, -1.f, 0.f,
	     0.5f, -0.5f, -0.5f,  texSize*1.f, texSize*1.f,  0.f, -1.f, 0.f,
	    -0.5f, -0.5f, -0.5f,  texSize*0.f, texSize*1.f,  0.f, -1.f, 0.f,

	    // top
	    -0.5f,  0.5f,  0.5f,  texSize*0.f, texSize*0.f,  0.f, 1.f, 0.f,
	    -0.5f,  0.5f, -0.5f,  texSize*0.f, texSize*1.f,  0.f, 1.f, 0.f,
	     0.5f,  0.5f, -0.5f,  texSize*1.f, texSize*1.f,  0.f, 1.f, 0.f,

	     0.5f,  0.5f, -0.5f,  texSize*1.f, texSize*1.f,  0.f, 1.f, 0.f,
	     0.5f,  0.5f,  0.5f,  texSize*1.f, texSize*0.f,  0.f, 1.f, 0.f,
	    -0.5f,  0.5f,  0.5f,  texSize*0.f, texSize*0.f,  0.f, 1.f, 0.f,
	};

}
//destroy the block
void Block::destroy() {
	delete[] vertices;
	vertices = nullptr;
}

void Block::draw() {
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

bool Block::isColorless(std::uint8_t blockID) {
	return (blockID == 0);
}

Block::Block() {

}

Block::~Block() {

}

