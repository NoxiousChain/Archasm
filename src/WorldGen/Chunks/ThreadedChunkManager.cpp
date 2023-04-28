//#include "ChunkManager.hpp"
//
//void ThreadedChunkManager::initialize(const String& saveName, int playerX, int screenWidth)
//{
//	Godot::print("ChunkManager::initialize()");
//	this->saveName = saveName;
//	//loadAllChunks(playerX, screenWidth);
//}
//
//void ThreadedChunkManager::_register_methods()
//{
//	register_method("initialize", &ThreadedChunkManager::initialize);
//	register_method("_ready", &ThreadedChunkManager::_ready);
//	register_method("set_save_name", &ThreadedChunkManager::setSaveName);
//	register_method("world_to_chunk_x", &ThreadedChunkManager::worldToChunkX);
//	register_method("chunk_to_world_x", &ThreadedChunkManager::chunkToWorldX);
//	
//	
//	/*
//	register_method("load_chunk", &ChunkManager::loadChunk);
//	register_method("save_chunk", &ChunkManager::saveChunk);
//	register_method("load_all_chunks", &ChunkManager::loadAllChunks);
//	register_method("save_all_chunks", &ChunkManager::saveAllChunks);
//	register_method("load_center_chunk", &ChunkManager::loadCenterChunk);
//	register_method("thread_load_chunk", &ChunkManager::threadLoadChunk);
//	register_method("thread_save_chunk", &ChunkManager::threadSaveChunk);
//	*/
//}
//
//void ThreadedChunkManager::_init()
//{
//	Godot::print("ChunkManager::_init()");
//
//	threads.resize(MAX_THREADS);
//	for (size_t i = 0; i < MAX_THREADS; i++) {
//		availableThreads.push(i);
//	}
//}
//
//void ThreadedChunkManager::_ready()
//{
//	Godot::print("ChunkManager::_ready()");
//	tg = TerrainGenerator::_new();
//}
//
//void ThreadedChunkManager::setSaveName(const String& saveName)
//{
//	this->saveName = saveName;
//}
//
//int ThreadedChunkManager::worldToChunkX(int worldX)
//{
//	return Chunk::worldToChunkX(worldX);
//}
//
//int ThreadedChunkManager::chunkToWorldX(int chunkX)
//{
//	return Chunk::chunkToWorldX(chunkX);
//}
//
//void ThreadedChunkManager::makeThreadAvailable(size_t index)
//{
//	MutexLock(queueMtx);
//	availableThreads.push(index);
//}
//
//size_t ThreadedChunkManager::getAvailableThread()
//{
//	MutexLock(queueMtx);
//	return availableThreads.pop();
//}
//
//void ThreadedChunkManager::loadAllChunks(int playerX, int screenW)
//{
//	// worldToChunkX(screenWidth) + 1 returns the number of chunks visible on the screen
//	int numChunks = (Chunk::worldToChunkX(screenW) + 1) + 2 * HIDDEN_CHUNKS;
//
//	// Load center chunk
//	loadChunkAtX(playerX);
//	for (int i = 0; i < numChunks / 2; i++) {
//		size_t index = getAvailableThread();
//		MutexLock(threadMtx);
//		threads[index].instance();
//		threads[index]->start(this, "load_chunk", true);
//	}
//	for (int i = 0; i < numChunks / 2; i++) {
//		size_t index = getAvailableThread();
//		MutexLock(threadMtx);
//		threads[index].instance();
//		threads[index]->start(this, "load_chunk", false);
//	}
//}
//
//void ThreadedChunkManager::loadChunkAtX(int worldX)
//{
//}
//
//void ThreadedChunkManager::loadChunk(bool right)
//{
//}
//
//void ThreadedChunkManager::saveAllChunks()
//{
//}
//
//void ThreadedChunkManager::saveChunk(bool right)
//{
//}
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
///*
//void ChunkManager::loadChunk(bool right)
//{
//	Godot::print("loadChunk() called");
//	if (right) {
//		// Create a new chunk and add it to the tilemaps
//		Chunk c(tileMaps.back().getX() + 1);
//		c.getTileMap()->set_visible(false);
//		add_child(c.getTileMap());
//		c.getTileMap()->set_owner(this);
//		tileMaps.push_back(c);
//	}
//	else {
//		// Create a new chunk and add it to the tilemaps
//		Chunk c(tileMaps.front().getX() - 1);
//		c.getTileMap()->set_visible(false);
//		add_child(c.getTileMap());
//		c.getTileMap()->set_owner(this);
//		tileMaps.push_front(std::move(c));
//	}
//
//	// Find available thread
//	int threadIndex = getAvailableThread();
//	if (threadIndex == -1) {
//		Godot::print("No available threads to load a chunk");
//		return;
//	}
//
//	// Mutexes have to be locked for r/w on thread vector, but cannot be locked during execution
//	threads[threadIndex].instance();
//	threads[threadIndex]->start(this, "thread_load_chunk", right);
//	freeThread(threadIndex);
//}
//
//void ChunkManager::saveChunk(bool right)
//{
//	int threadIndex = getAvailableThread();
//	if (threadIndex == -1) {
//		Godot::print("No available threads to save a chunk");
//		return;
//	}
//
//	threads[threadIndex].instance();
//	threads[threadIndex]->start(this, "thread_save_chunk", right);
//	freeThread(threadIndex);
//
//	if (right) {
//		remove_child(tileMaps.back().getTileMap());
//		tileMaps.pop_back();
//		// Update visible tilemaps
//		tileMaps[tileMaps.size() - (HIDDEN_CHUNKS + 1)].getTileMap()->set_visible(false);
//		tileMaps[HIDDEN_CHUNKS - 1].getTileMap()->set_visible(true);
//	}
//	else {
//		remove_child(tileMaps.front().getTileMap());
//		tileMaps.pop_front();
//		// Update visible tilemaps
//		tileMaps[HIDDEN_CHUNKS - 1].getTileMap()->set_visible(false);
//		tileMaps[tileMaps.size() - (HIDDEN_CHUNKS + 1)].getTileMap()->set_visible(true);
//	}
//}
//
//void ChunkManager::loadCenterChunk(int x)
//{
//	// Create a new chunk and add it to the tilemaps
//	tileMaps.push_back(Chunk(x));
//	tileMaps.back().getTileMap()->set_visible(true);
//
//	// Just load directly because its only one chunk & happens only on startup
//	tileMaps.back().load(saveName, tg);
//}
//
//void ChunkManager::loadAllChunks(int playerX, int screenWidth)
//{
//	Godot::print("loading all chunks...");
//	int centerChunk = Chunk::worldToChunkX(playerX);
//	// worldToChunkX(screenWidth) + 1 returns the number of chunks visible on the screen
//	int numChunks = (Chunk::worldToChunkX(screenWidth) + 1) + 2*HIDDEN_CHUNKS;
//
//	// First load the center chunk, because other functions need at least 1 existing chunk to work properly
//	loadCenterChunk(centerChunk);
//	Godot::print("center chunk loaded");
//	// Load all chunks to the right on threads
//	for (int i = centerChunk + 1; i < centerChunk + numChunks / 2; i++) {
//		loadChunk(true);
//	}
//	// Load all chunks to the left on threads
//	for (int i = centerChunk - 1; i > centerChunk - numChunks / 2; i--) {
//		loadChunk(false);
//	}
//	
//	// Hide invisible chunks
//	for (int i = 0; i < HIDDEN_CHUNKS; i++) {
//		tileMaps.at(i).getTileMap()->hide();
//		tileMaps.at(tileMaps.size() - (i + 1)).getTileMap()->hide();
//	}
//}
//
//// Need to rewrite to support threading
//void ChunkManager::saveAllChunks()
//{
//	std::vector<std::thread> saveThreads;
//
//	for (Chunk& chunk : tileMaps) {
//		saveThreads.push_back(std::thread([&] {
//			chunk.save(saveName);
//		}));
//	}
//	
//	for (auto& saveThread : saveThreads) {
//		saveThread.join();
//	}
//}
//
//int ChunkManager::getAvailableThread()
//{
//	mtx->lock();
//	for (size_t i = 0; i < availableThreads.size(); i++) {
//		if (availableThreads[i]) {
//			availableThreads[i] = false;
//			return i;
//		}
//	}
//	mtx->unlock();
//	return -1; // No available threads
//}
//
//void ChunkManager::freeThread(int index)
//{
//	if (threads[index].is_valid()) {
//		threads[index]->wait_to_finish();
//		threads[index].unref();
//		availableThreads[index] = true;
//	}
//}
//
//void ChunkManager::threadSaveChunk(bool right)
//{
//	if (right) {
//		tileMaps.back().save(saveName);
//	}
//	else {
//		tileMaps.front().save(saveName);
//	}
//}
//
//void ChunkManager::threadLoadChunk(bool right)
//{
//	if (right) {
//		tileMaps.back().load(saveName, tg);
//	}
//	else {
//		tileMaps.front().load(saveName, tg);
//	}
//}
//*/