#include "ChunkManager.hpp"

ChunkManager* ChunkManager::create(const String& saveName, int playerX, int screenWidth)
{
	ChunkManager* cm = ChunkManager::_new();
	cm->initialize(saveName, playerX, screenWidth);
	return cm;
}

void ChunkManager::initialize(const String& saveName, int playerX, int screenWidth)
{
	this->saveName = saveName;
	loadAllChunks(playerX, screenWidth);
}

void ChunkManager::_register_methods()
{
	register_method("initialize", &ChunkManager::initialize);
	register_method("_ready", &ChunkManager::_ready);
	register_method("set_save_name", &ChunkManager::setSaveName);
	register_method("load_chunk", &ChunkManager::loadChunk);
	register_method("save_chunk", &ChunkManager::saveChunk);
	register_method("load_all_chunks", &ChunkManager::loadAllChunks);
	register_method("save_all_chunks", &ChunkManager::saveAllChunks);
	register_method("world_to_chunk_x", &ChunkManager::worldToChunkX);
	register_method("chunk_to_world_x", &ChunkManager::chunkToWorldX);
}

void ChunkManager::_init()
{
}

void ChunkManager::_ready()
{
	tg = TerrainGenerator::_new();
}

void ChunkManager::setSaveName(const String& saveName)
{
	this->saveName = saveName;
}

void ChunkManager::loadChunk(bool right)
{
	if (right) {
		tileMaps.push_back(Chunk(tileMaps.back().getX() + 1));
	}
	else {
		tileMaps.push_front(Chunk(tileMaps.front().getX() - 1));
	}

	std::mutex mtx;
	std::condition_variable cv;
	bool loadFinished = false;

	std::thread loadThread([this, &mtx, &cv, &loadFinished, right] {
		this->threadLoadChunk(mtx, cv, loadFinished, right);
	});
	{
		std::unique_lock<std::mutex> lock(mtx);
		cv.wait(lock, [&] { return loadFinished; });
	}
	loadThread.join();
}

void ChunkManager::saveChunk(bool right)
{
	std::mutex mtx;
	std::condition_variable cv;
	bool saveFinished = false;

	std::thread saveThread([this, &mtx, &cv, &saveFinished, right] {
		this->threadSaveChunk(mtx, cv, saveFinished, right);
	});
	{
		std::unique_lock<std::mutex> lock(mtx);
		cv.wait(lock, [&] { return saveFinished; });
	}

	if (right) {
		tileMaps.pop_back();
	}
	else {
		tileMaps.pop_front();
	}

	saveThread.join();
}

void ChunkManager::loadAllChunks(int playerX, int screenWidth)
{
	int centerChunk = worldToChunkX(playerX);
	int numChunks = worldToChunkX(screenWidth) + 2*HIDDEN_CHUNKS;

	// First load the center chunk, because other functions need at least 1 existing chunk to work properly
	loadChunk(centerChunk);
	// Load all chunks to the right on threads
	for (int i = centerChunk + 1; i < centerChunk + numChunks / 2; i++) {
		loadChunk(true);
	}
	// Load all chunks to the left on threads
	for (int i = centerChunk - 1; i > centerChunk - numChunks / 2; i--) {
		loadChunk(false);
	}
	
	// Hide invisible chunks
	for (int i = 0; i < HIDDEN_CHUNKS; i++) {
		tileMaps.at(i).getTileMap()->hide();
		tileMaps.at(tileMaps.size() - (i + 1)).getTileMap()->hide();
	}
}

// Need to rewrite to support threading
void ChunkManager::saveAllChunks()
{
	std::vector<std::thread> saveThreads;

	for (Chunk& chunk : tileMaps) {
		saveThreads.push_back(std::thread([&] {
			chunk.save(saveName);
		}));
	}
	
	for (auto& saveThread : saveThreads) {
		saveThread.join();
	}
}

int ChunkManager::worldToChunkX(int worldX)
{
	return worldX / (CELL_SIZE * CHUNK_WIDTH);
}

int ChunkManager::chunkToWorldX(int chunkX)
{
	return chunkX * CELL_SIZE * CHUNK_WIDTH;
}

void ChunkManager::threadSaveChunk(std::mutex& mtx, std::condition_variable& cv, bool& saveFinished, bool right)
{
	if (right) {
		tileMaps.back().save(saveName);
	}
	else {
		tileMaps.back().save(saveName);
	}

	std::unique_lock<std::mutex> lock(mtx);
	saveFinished = true;
	cv.notify_one();
}

void ChunkManager::threadLoadChunk(std::mutex& mtx, std::condition_variable& cv, bool& loadFinished, bool right)
{
	if (right) {
		tileMaps.back().load(saveName, tg);
	}
	else {
		tileMaps.front().load(saveName, tg);
	}
	std::unique_lock<std::mutex> lock(mtx);
	loadFinished = true;
	cv.notify_one();
}

