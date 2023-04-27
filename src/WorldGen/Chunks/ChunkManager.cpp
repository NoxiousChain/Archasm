#include "ChunkManager.hpp"

ChunkManager* ChunkManager::create(const String& saveName, int playerX, int screenWidth)
{
	ChunkManager* cm = ChunkManager::_new();
	cm->initialize(saveName, playerX, screenWidth);
	return cm;
}

void ChunkManager::initialize(const String& saveName, int playerX, int screenWidth)
{
	Godot::print("ChunkManager::initialize()");
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
	register_method("load_center_chunk", &ChunkManager::loadCenterChunk);
	register_method("world_to_chunk_x", &ChunkManager::worldToChunkX);
	register_method("chunk_to_world_x", &ChunkManager::chunkToWorldX);
}

void ChunkManager::_init()
{
	Godot::print("ChunkManager::_init()");

	threads.resize(MAX_THREADS);
	availableThreads.resize(MAX_THREADS, true);
}

void ChunkManager::_ready()
{
	Godot::print("ChunkManager::_ready()");
	tg = TerrainGenerator::_new();
}

void ChunkManager::setSaveName(const String& saveName)
{
	this->saveName = saveName;
}

void ChunkManager::loadChunk(bool right)
{
	Godot::print("loadChunk() called");
	if (right) {
		// Create a new chunk and add it to the tilemaps
		Chunk c(tileMaps.back().getX() + 1);
		c.getTileMap()->set_visible(false);
		add_child(c.getTileMap());
		c.getTileMap()->set_owner(this);
		tileMaps.push_back(c);
	}
	else {
		// Create a new chunk and add it to the tilemaps
		Chunk c(tileMaps.front().getX() - 1);
		c.getTileMap()->set_visible(false);
		add_child(c.getTileMap());
		c.getTileMap()->set_owner(this);
		tileMaps.push_front(std::move(c));
	}

	// Find available thread
	int threadIndex = getAvailableThread();
	if (threadIndex == -1) {
		Godot::print_error("No available threads to load a chunk", __FILE__, __LINE__);
		return;
	}

	threads[threadIndex] = Thread::_new();
	threads[threadIndex]->start([this, right, threadIndex] {
		this->threadLoadChunk(right);
		mtx->lock();
		freeThread(threadIndex);
		mtx->unlock();
	});

}

void ChunkManager::saveChunk(bool right)
{
	int threadIndex = getAvailableThread();
	if (threadIndex == -1) {
		Godot::print_error("No available threads to save a chunk", __FILE__, __LINE__);
		return;
	}

	threads[threadIndex] = Thread::_new();
	threads[threadIndex]->start(this, "thread_load_chunk", right)
	threads[threadIndex]->start([this, right, threadIndex] {
		this->threadLoadChunk(right);
		mtx->lock();
		freeThread(threadIndex);
		mtx->unlock();
	});

	if (right) {
		remove_child(tileMaps.back().getTileMap());
		tileMaps.pop_back();
		// Update visible tilemaps
		tileMaps[tileMaps.size() - (HIDDEN_CHUNKS + 1)].getTileMap()->set_visible(false);
		tileMaps[HIDDEN_CHUNKS - 1].getTileMap()->set_visible(true);
	}
	else {
		remove_child(tileMaps.front().getTileMap());
		tileMaps.pop_front();
		// Update visible tilemaps
		tileMaps[HIDDEN_CHUNKS - 1].getTileMap()->set_visible(false);
		tileMaps[tileMaps.size() - (HIDDEN_CHUNKS + 1)].getTileMap()->set_visible(true);
	}
}

void ChunkManager::loadCenterChunk(int x)
{
	// Create a new chunk and add it to the tilemaps
	tileMaps.push_back(Chunk(x));
	tileMaps.back().getTileMap()->set_visible(true);

	// Just load directly because its only one chunk & happens only on startup
	tileMaps.back().load(saveName, tg);
}

void ChunkManager::loadAllChunks(int playerX, int screenWidth)
{
	Godot::print("loading all chunks...");
	int centerChunk = Chunk::worldToChunkX(playerX);
	int numChunks = Chunk::worldToChunkX(screenWidth) + 2*HIDDEN_CHUNKS;

	// First load the center chunk, because other functions need at least 1 existing chunk to work properly
	loadCenterChunk(centerChunk);
	Godot::print("center chunk loaded");
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
	for (int i = HIDDEN_CHUNKS; i < tileMaps.size(); i++) {
		tileMaps.at(i).getTileMap()->show();
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
	return Chunk::worldToChunkX(worldX);
}

int ChunkManager::chunkToWorldX(int chunkX)
{
	return Chunk::chunkToWorldX(chunkX);
}

int ChunkManager::getAvailableThread()
{
	mtx->lock();
	for (size_t i = 0; i < availableThreads.size(); i++) {
		if (availableThreads[i]) {
			availableThreads[i] = false;
			return i;
		}
	}
	mtx->unlock();
	return -1; // No available threads
}

void ChunkManager::freeThread(int index)
{
	if (threads[index].is_valid()) {
		threads[index]->wait_to_finish();
		threads[index].unref();
		availableThreads[index] = true;
	}
}

void ChunkManager::threadSaveChunk(bool right)
{
	if (right) {
		tileMaps.back().save(saveName);
	}
	else {
		tileMaps.front().save(saveName);
	}
}

void ChunkManager::threadLoadChunk(bool right)
{
	if (right) {
		tileMaps.back().load(saveName, tg);
	}
	else {
		tileMaps.front().load(saveName, tg);
	}
}

/*
void ChunkManager::threadSaveChunk(std::mutex& mtx, std::condition_variable& cv, bool& saveFinished, bool right)
{
	if (right) {
		tileMaps.back().save(saveName);
	}
	else {
		tileMaps.front().save(saveName);
	}
	std::unique_lock<std::mutex> lock(mtx);
	loadFinished = true;
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
*/

