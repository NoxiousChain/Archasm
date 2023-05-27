#include "ChunkManager.hpp"

ChunkManager::~ChunkManager()
{
	assert(pool != nullptr);
	pool->wait(); // wait for all current threads to close
	saveAllChunks();
	pool->wait(); // wait for save threads to close 
	deleteAllChunks(); // delete any leftover chunks
	delete pool; // clean up threads

	tg->queue_free();
}

void ChunkManager::_register_methods()
{
	string threads_desc = "THREAD_EXPONENT (This property sets the number of threads as 2^n. E.g., if EXPONENT is 3, the number of threads will be 2^3 = 8)";
	/***/
	register_property("Threading Parameters", &ChunkManager::THREADING_PARAMETERS, CATEGORY_SEPARATOR, GODOT_METHOD_RPC_MODE_DISABLED, GODOT_PROPERTY_USAGE_CATEGORY);
	register_property(threads_desc.c_str(), &ChunkManager::THREAD_EXP, size_t(3), GODOT_METHOD_RPC_MODE_DISABLED, GODOT_PROPERTY_USAGE_DEFAULT, GODOT_PROPERTY_HINT_RANGE, "0,6,1");
	/***/
	register_property("Cave Generation Parameters", &ChunkManager::CAVE_GENERATION_PARAMETERS, CATEGORY_SEPARATOR, GODOT_METHOD_RPC_MODE_DISABLED, GODOT_PROPERTY_USAGE_CATEGORY);
	register_property("CAVE_CHANCE_TO_START_ALIVE", &ChunkManager::CAVE_CHANCE_TO_START_ALIVE, 0.5f, GODOT_METHOD_RPC_MODE_DISABLED, GODOT_PROPERTY_USAGE_DEFAULT, GODOT_PROPERTY_HINT_RANGE, "0,1");
	register_property("CAVE_MAX_HEIGHT_WEIGHT", &ChunkManager::CAVE_MAX_HEIGHT_WEIGHT, 0.35f, GODOT_METHOD_RPC_MODE_DISABLED, GODOT_PROPERTY_USAGE_DEFAULT, GODOT_PROPERTY_HINT_RANGE, "0,1");
	register_property("CAVE_THRESHOLD", &ChunkManager::CAVE_THRESHOLD, 0.3f, GODOT_METHOD_RPC_MODE_DISABLED, GODOT_PROPERTY_USAGE_DEFAULT, GODOT_PROPERTY_HINT_RANGE, "0,1");
	register_property("CAVE_THRESHOLD_Y_MODIFIER", &ChunkManager::CAVE_THRESHOLD_Y_MODIFIER, 0.4f, GODOT_METHOD_RPC_MODE_DISABLED, GODOT_PROPERTY_USAGE_DEFAULT, GODOT_PROPERTY_HINT_RANGE, "0,1");
	/***/

	register_method("_ready", &ChunkManager::_ready);
	register_method("initialize", &ChunkManager::initialize);
	register_method("update", &ChunkManager::update);
	register_method("set_save_name", &ChunkManager::setSaveName);
	register_method("world_to_chunk_x", &ChunkManager::worldToChunkX);
	register_method("chunk_to_world_x", &ChunkManager::chunkToWorldX);
	register_method("get_num_chunks", &ChunkManager::getNumChunks);
	register_method("load_all_chunks", &ChunkManager::loadAllChunks);
	register_method("load_chunk", &ChunkManager::tryLoadChunk);
	register_method("save_all_chunks", &ChunkManager::saveAllChunks);
	register_method("save_chunk", &ChunkManager::saveChunkAtX);
	register_method("delete_chunk", &ChunkManager::deleteChunkAtX);
	register_method("delete_all_chunks", &ChunkManager::deleteAllChunks);
}

void ChunkManager::_init()
{
}

void ChunkManager::_ready()
{
	tg = TerrainGenerator::_new();
	tg->_ready(); // _ready() is never called automatically for some reason, so I do it manually here
	tg->setParent(this);

	tileMapScene = ResourceLoader::get_singleton()->load("res://scenes/Chunk.tscn");
	assert(tileMapScene != nullptr);

	pool = new ThreadPool((size_t)exp2(THREAD_EXP));

	ofstream logFile("C:\\Users\\Forest\\Desktop\\log.txt", ios_base::app);
	logFile << "ChunkManager::_ready() " << endl;
	logFile.close();
}

void ChunkManager::initialize(String saveName, int playerX, int screenW)
{
	this->saveName = saveName;
	loadAllChunks(playerX, screenW); // loadAllChunks needs to be fixed. Not loading properly
}

void ChunkManager::update()
{
	ofstream logFile;
	logFile.open("C:\\Users\\Forest\\Desktop\\log.txt", ios_base::app);
	logFile << "ChunkManager::update()" << endl;
	logFile.close();
	lock_guard<mutex> lock(mtx);
	// Check all futures
	for (auto& it = loadingChunks.begin(); it != loadingChunks.end();) {
		auto& future = it->second;
		// If future is ready
		if (future.wait_for(chrono::seconds(0)) == future_status::ready) {
			ofstream logFile;
			logFile.open("C:\\Users\\Forest\\Desktop\\log.txt", ios_base::app);
			logFile << "future ready" << endl;
			logFile.close();

			shared_ptr<Chunk> chunk = loadedChunks[it->first];
			chunk->instance(tileMapScene);
			chunk->getTileMap()->set_cell_size(Vector2(CELL_SIZE, CELL_SIZE));
			chunk->getTileMap()->set_position(Vector2(real_t(chunkToWorldX(chunk->getX())), 0.f));
			applyChunkData(it->first, future.get());
			it = loadingChunks.erase(it); // remove future from map
		}
		else
			it++;
	}
	
	// Save all chunks queued for save
	for (auto& it = queuedForSave.begin(); it != queuedForSave.end();) {
		saveChunkAtX(*it);
		queuedForSave.erase(it);
	}

	// Delete all chunks queued for deletion
	for (auto& it = queuedForDeletion.begin(); it != queuedForDeletion.end();) {
		if (deleteChunkAtX(*it))
			it = queuedForDeletion.erase(it);
		else
			it++;
	}
}

void ChunkManager::setSaveName(String saveName)
{
	this->saveName = saveName;
}

int ChunkManager::worldToChunkX(int worldX)
{
	return Chunk::worldToChunkX(worldX);
}

int ChunkManager::chunkToWorldX(int chunkX)
{
	return Chunk::chunkToWorldX(chunkX);
}

int ChunkManager::getNumChunks()
{
	lock_guard<mutex> lock(mtx);
	int count = int(queuedChunks.size() + loadedChunks.size());
	Godot::print(String("numChunks: ") + String::num_int64(count));
	return count;
}

void ChunkManager::tryLoadChunk(int chunkX)
{
	unique_lock<mutex> lock(mtx);
	// Don't load a chunk that exists/is already being loaded
	bool exists = queuedChunks.find(chunkX) != queuedChunks.end()
		|| loadedChunks.find(chunkX) != loadedChunks.end()
		|| loadingChunks.find(chunkX) != loadingChunks.end();
	if (!exists) {
		queuedChunks.insert(chunkX);
		auto pr = make_shared<promise<chunkdata_t>>();
		auto future = pr->get_future();
		loadingChunks[chunkX] = move(future);
		lock.unlock();
		pool->enqueue([this, chunkX, p = pr]() mutable { 
			chunkdata_t data = this->loadChunkAtX(chunkX);
			p->set_value(move(data));
		});
	}
}

void ChunkManager::loadAllChunks(int playerX, int screenW)
{
	int chunkX = Chunk::worldToChunkX(playerX);
	// worldToChunkX(screenW) + 1 returns the number of chunks visible on the screen
	int numChunks = (Chunk::worldToChunkX(screenW) + 1) + 2 * HIDDEN_CHUNKS;
	// [-3, 5]
	for (int i = chunkX - (numChunks / 2 + 1); i <= numChunks / 2 + 1; i++) {
		tryLoadChunk(i);
		ofstream logFile;
		logFile.open("C:\\Users\\Forest\\Desktop\\log.txt", ios_base::app);
		logFile << "Successfully queued chunk " << i << endl;
		logFile << "Total chunks in queue: " << queuedChunks.size() << endl;
		logFile.close();
	}
}

chunkdata_t ChunkManager::loadChunkAtX(int chunkX)
{
	// Mutex locked function calls
	{
		lock_guard<mutex> lock(mtx);
		queuedChunks.erase(chunkX);
	}

	// Inherently thread safe function calls
	auto c = make_shared<Chunk>(chunkX);
	chunkdata_t data = c->load(saveName, tg);

	// Deferred function calls
	call_deferred("add_child", Array::make(c->getTileMap()));
	c->getTileMap()->call_deferred("set_owner", Array::make(this));
	
	// More mutex locking
	{
		lock_guard<mutex> lock(mtx);
		get_parent()->get_parent()->call_deferred("connect", Array::make(SIGNAL_TOGGLE_INTERACT, c->getTileMap(), "_toggle_interact"));

		loadedChunks.insert({ chunkX, c });
	}

	return data;
}

void ChunkManager::applyChunkData(int chunkX, const chunkdata_t& data)
{
	lock_guard<mutex> lock(mtx);
	TileMap* tm = loadedChunks[chunkX]->getTileMap();
	for (auto& block : data) {
		tm->set_cell(block.x, block.y, block.id);
	}
}

void ChunkManager::saveAllChunks()
{
	lock_guard<mutex> lock(mtx);
	for (auto& chunk : loadedChunks) {
		auto ptr = chunk.second;
		pool->enqueue([this, ptr]() { this->saveChunk(ptr); });
	}
}

void ChunkManager::saveChunkAtX(int chunkX)
{
	lock_guard<mutex> lock(mtx);
	// If it is loaded, save it
	bool exists = loadedChunks.find(chunkX) != loadedChunks.end();
	if (exists) {
		auto chunk = loadedChunks[chunkX];
		pool->enqueue([this, chunk]() { this->saveChunk(chunk); });
	}
	// If it is in load queue, queue for saving
	else if (queuedChunks.find(chunkX) != queuedChunks.end() || loadingChunks.find(chunkX) != loadingChunks.end()) {
		queuedForSave.insert(chunkX);
	}
	else // doesn't exist
		Godot::print(String("Attempting to save a nonexistent chunk (chunk: ") + String::num_int64(chunkX) + String(")"));
}

void ChunkManager::saveChunk(shared_ptr<Chunk> chunk)
{
	chunk->save(saveName);
}

bool ChunkManager::deleteChunkAtX(int chunkX)
{
	// If it is loaded, delete it and remove it from the scene
	if (loadedChunks.find(chunkX) != loadedChunks.end()) {
		call_deferred("remove_child", Array::make(loadedChunks[chunkX]->getTileMap()));
		loadedChunks.erase(chunkX);
	}
	// If it is queued to be loaded or currently being loaded, just queue it back for deletion
	// It will be deleted next update cycle
	else if (queuedChunks.find(chunkX) != queuedChunks.end() || loadingChunks.find(chunkX) != loadingChunks.end()) {
		return false;
	}
	// Otherwise the chunk doesn't exist
	else {
		Godot::print(String("Attempting to delete a nonexistent chunk (chunkX: ") + String::num_int64(chunkX) + String(")"));
		// Because it doesn't exist, we should remove it from the queue. 
		// This shouldn't be reached, consider it an error if so
	}
	return true;
}

void ChunkManager::deleteAllChunks()
{
	lock_guard<mutex> lock(mtx);
	for (auto& chunk : loadedChunks) {
		call_deferred("remove_child", Array::make(chunk.second->getTileMap()));
		// Chunks handle their own tilemap deletion
	}
	loadedChunks.clear();
}
