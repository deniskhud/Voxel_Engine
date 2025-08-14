#include "world.h"

/*World::World() {


}*/

World::~World() {
	for (auto& [key, value] : chunks) {
		delete value; // освободить память
	}
	chunks.clear();
}

void World::addChunk(int x, int z) {
	auto* chunk = new Chunk(x, z);
	chunks[{x, z}] = chunk;

	pool.enqueue([chunk]() {
		chunk->generateAsync();
	});

}

void World::deleteChunk(int x, int z) {
	delete chunks[{x,z}];
	chunks.erase({x,z});
}
int renderedChunks = 0;
void World::draw(Shader &shader, Camera &camera) {

	for (auto& [key, chunk] : chunks) {
		glm::vec3 pos = glm::vec3(key.first * 16, 0.f, key.second * 16);
		chunk->draw(shader, pos, camera, renderedChunks);
	}
	std::cout << "chunks: " << chunks.size() << std::endl;
	std::cout << "renderedChunks: " << renderedChunks << std::endl;
	renderedChunks = 0;
}

void World::setPlayerPosition(glm::vec3 pos) {
	camera_position = pos;

	int currentChunkX = static_cast<int>(floor(pos.x / 16));
	int currentChunkZ = static_cast<int>(floor(pos.z / 16));

	static std::pair<int,int> lastChunk = {9999,9999};
	if (lastChunk.first != currentChunkX || lastChunk.second != currentChunkZ) {
		// Создаем чанки вокруг игрока

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

void World::update() {
	// Проверяем готовые меши от генерации чанков
	while (true) {
		std::lock_guard<std::mutex> lock(readyMutex);
		if (readyMeshes.empty()) break;

		auto data = std::move(readyMeshes.front());
		readyMeshes.pop();

		auto it = chunks.find({data.chunkX, data.chunkZ});
		if (it != chunks.end() && it->second) {
			it->second->uploadMeshToGPU(data.vertices);
		}
	}
}