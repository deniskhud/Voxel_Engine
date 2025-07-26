#include "world.h"

World::World() {
	int startX = 0;
	int startZ = 0;
	int size = 10; // или renderDistance

	for (int x = startX; x <= size; ++x) {
		for (int z = startZ; z <= size; ++z) {
			addChunk(x, z);
		}
	}
}

World::~World() {
	for (auto& [key, value] : chunks) {
		delete value; // освободить память
	}
	chunks.clear();
}

void World::addChunk(int x, int z) {
	chunks[{x, z}] = new Chunk();

}

void World::deleteChunk(int x, int z) {
	delete chunks[{x,z}];
	chunks.erase({x,z});
}

void World::draw(Shader &shader) {
	for (auto& [key, chunk] : chunks) {
		glm::vec3 pos = glm::vec3(key.first * 16, 0.f, key.second * 16);
		chunk->draw(shader, pos);
	}
}

void World::setPlayerPosition(glm::vec3 pos) {
	camera_position = pos;

	int currentChunkX = static_cast<int>(floor(pos.x / 16));
	int currentChunkZ = static_cast<int>(floor(pos.z / 16));

	static std::pair<int,int> lastChunk = {9999,9999};
	if (lastChunk.first != currentChunkX || lastChunk.second != currentChunkZ) {
		// Создаем чанки вокруг игрока
		int renderDistance = 5;
		for (int x = currentChunkX - renderDistance; x <= currentChunkX + renderDistance; ++x) {
			for (int z = currentChunkZ - renderDistance; z <= currentChunkZ + renderDistance; ++z) {
				if (chunks.find({x,z}) == chunks.end()) {
					addChunk(x, z);
				}
			}
		}

		// Удаляем чанки, которые далеко
		for (auto it = chunks.begin(); it != chunks.end();) {
			int dx = abs(it->first.first - currentChunkX);
			int dz = abs(it->first.second - currentChunkZ);
			if (dx > renderDistance || dz > renderDistance) {
				delete it->second;
				it = chunks.erase(it);
			} else {
				++it;
			}
		}

		lastChunk = {currentChunkX, currentChunkZ};
	}
}
