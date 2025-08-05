#ifndef WORLD_H
#define WORLD_H

#include <condition_variable>

#include "chunk.h"
#include "camera.h"
#include "shader.h"
#include <unordered_map>
#include <thread>
#include <mutex>

struct pair_hash {
	template <class T1, class T2>
	std::size_t operator () (const std::pair<T1,T2> &p) const {
		auto h1 = std::hash<T1>{}(p.first);
		auto h2 = std::hash<T2>{}(p.second);
		return h1 ^ (h2 << 1);
	}
};

class World {
public:
	World();
	~World();

	void addChunk(int x, int z);
	void deleteChunk(int x, int z);

	void draw(Shader& shader);
	void setPlayerPosition(glm::vec3 pos);


private:
	glm::vec3 camera_position;
	//std::unordered_map<int, Chunk*> chunks;
	std::unordered_map<std::pair<int, int>, Chunk*, pair_hash> chunks;
	const int renderDistance = 8;

	std::thread renderThread;

	std::condition_variable cv;
	std::mutex mtx;

	bool running = false;
	void threadFunc();
};


#endif //WORLD_H