#include "world.h"

World::World() {
	for (int i = 0; i <= 64; ++i) {
		addChunk(i);
	}
}

World::~World() {
	for (auto& [key, value] : chunks) {
		deleteChunk(key);
	}
}

void World::addChunk(int x) {
	chunks[x] = new Chunk();
}

void World::deleteChunk(int x) {
	chunks.erase(x);
}

void World::draw(Shader &shader) {
	for (auto& [key, value] : chunks) {
		value->draw(shader, glm::vec3((key / 8) * 32.f, 0.f, (key % 8) * 32.f));
	}
}

void World::setPlayerPosition(glm::vec3 pos) {
	camera_position = pos;
}
