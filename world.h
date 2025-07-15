#ifndef WORLD_H
#define WORLD_H

#include "chunk.h"
#include "camera.h"
#include "shader.h"
#include <unordered_map>

class World {
public:
	World();
	~World();

	void addChunk(int x);
	void deleteChunk(int x);

	void draw(Shader& shader);
	void setPlayerPosition(glm::vec3 pos);


private:
	glm::vec3 camera_position;
	std::unordered_map<int, Chunk*> chunks;
};


#endif //WORLD_H
