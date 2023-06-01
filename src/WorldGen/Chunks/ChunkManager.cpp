#include "ChunkManager.hpp"

ChunkManager::~ChunkManager()
{
	assert(pool);
	pool->wait();
	saveAllChunks();
	pool->wait();
	tg->queue_free();
}

void ChunkManager::_register_methods()
{
	/* GENERATION PROPERTIES */

	// Threading
	string threads_desc = "THREAD_EXPONENT (This property sets the number of threads as 2^n. E.g., if EXPONENT is 3, the number of threads will be 2^3 = 8)";
	register_property("Threading Parameters", &ChunkManager::THREADING_PARAMETERS, CATEGORY_SEPARATOR, GODOT_METHOD_RPC_MODE_DISABLED, GODOT_PROPERTY_USAGE_CATEGORY);
	register_property(threads_desc.c_str(), &ChunkManager::THREAD_EXP, size_t(3), GODOT_METHOD_RPC_MODE_DISABLED, GODOT_PROPERTY_USAGE_DEFAULT, GODOT_PROPERTY_HINT_RANGE, "0,6,1");

	// Caves
	register_property("Cave Generation Parameters", &ChunkManager::CAVE_GENERATION_PARAMETERS, CATEGORY_SEPARATOR, GODOT_METHOD_RPC_MODE_DISABLED, GODOT_PROPERTY_USAGE_CATEGORY);
	register_property("CAVE_CHANCE_TO_START_ALIVE", &ChunkManager::CAVE_CHANCE_TO_START_ALIVE, 0.5f, GODOT_METHOD_RPC_MODE_DISABLED, GODOT_PROPERTY_USAGE_DEFAULT, GODOT_PROPERTY_HINT_RANGE, "0,1");
	register_property("CAVE_MAX_HEIGHT_WEIGHT", &ChunkManager::CAVE_MAX_HEIGHT_WEIGHT, 0.35f, GODOT_METHOD_RPC_MODE_DISABLED, GODOT_PROPERTY_USAGE_DEFAULT, GODOT_PROPERTY_HINT_RANGE, "0,1");
	register_property("CAVE_THRESHOLD", &ChunkManager::CAVE_THRESHOLD, 0.3f, GODOT_METHOD_RPC_MODE_DISABLED, GODOT_PROPERTY_USAGE_DEFAULT, GODOT_PROPERTY_HINT_RANGE, "0,1");
	register_property("CAVE_THRESHOLD_Y_MODIFIER", &ChunkManager::CAVE_THRESHOLD_Y_MODIFIER, 0.4f, GODOT_METHOD_RPC_MODE_DISABLED, GODOT_PROPERTY_USAGE_DEFAULT, GODOT_PROPERTY_HINT_RANGE, "0,1");
	

	/* METHODS */

	register_method("_ready", &ChunkManager::_ready);
	register_method("initialize", &ChunkManager::initialize);
	register_method("update", &ChunkManager::update);
	register_method("set_save_name", &ChunkManager::setSaveName);
	register_method("world_to_chunk_x", &ChunkManager::worldToChunkX);
	register_method("chunk_to_world_x", &ChunkManager::chunkToWorldX);
	register_method("load_chunk_at_x", &ChunkManager::loadChunkAtX);
	register_method("save_all_chunks", &ChunkManager::saveAllChunks);
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
	tileMapScene.instance();
	assert(tileMapScene.is_valid());

	pool = make_unique<ThreadPool>(pow(2, THREAD_EXP));
}

void ChunkManager::initialize(String saveName, int playerX, int screenW)
{
	this->saveName = saveName;
	auto newChunk = make_shared<Chunk>(0, true);
	chunks.emplace_front(LOADED, newChunk);
	loadChunkAtX(0);
	update(playerX, screenW);
}

void ChunkManager::update(int playerX, int screenW)
{
	int chunkX = worldToChunkX(playerX);
	int numChunks = (worldToChunkX(screenW) + 1) + 2 * HIDDEN_CHUNKS;
	int leftChunkX = chunkX - numChunks / 2;
	int rightChunkX = chunkX + numChunks / 2;

	deque<pair<ChunkState, shared_ptr<Chunk>>> chunksToLoad;
	deque<pair<ChunkState, shared_ptr<Chunk>>> chunksToSave;

	{
		lock_guard<mutex> lock(mtx);

		for (int i = chunks.front().second->getX(); i > leftChunkX; i--) {
			if (getStateAtChunkX(i) != LOADING) {
				auto newChunk = make_shared<Chunk>(i - 1, true);
				chunks.emplace_front(LOADING, newChunk);
				chunksToLoad.push_back({ LOADING, newChunk });
			}
		}

		for (int i = chunks.back().second->getX(); i < rightChunkX; i++) {
			if (getStateAtChunkX(i, true) != LOADING) {
				auto newChunk = make_shared<Chunk>(i + 1, true);
				chunks.emplace_back(LOADING, newChunk);
				chunksToLoad.push_back({ LOADING, newChunk });
			}
		}

		for (auto& c : loaded) {
			add_child(c->getTileMap());
			c->getTileMap()->set_owner(this);
			c->getTileMap()->set_position(Vector2(real_t(chunkToWorldX(c->getX())), 0.f));
		}
		loaded.clear();

		for (int i = chunks.front().second->getX(); i < leftChunkX; i++) {
			auto chk = chunks.front();
			chunks.pop_front();
			if (chk.first != SAVING) {
				chunksToSave.push_back(chk);
			}
		}

		for (int i = chunks.back().second->getX(); i > rightChunkX; i--) {
			auto chk = chunks.back();
			chunks.pop_back();
			if (chk.first != SAVING) {
				chunksToSave.push_back(chk);
			}
		}
	}

	for (auto& chk : chunksToLoad) {
		pool->enqueue([this, chk]() mutable {
			loadChunkAtX(chk.second->getX());
			});
	}

	for (auto& chk : chunksToSave) {
		pool->enqueue([this, chk]() mutable {
			chk.first = SAVING;
			saveChunk(chk.second);
			});
	}
}

void ChunkManager::setSaveName(String saveName)
{
	this->saveName = saveName;
}

int ChunkManager::worldToChunkX(int worldX) const
{
	return Chunk::worldToChunkX(worldX);
}

int ChunkManager::chunkToWorldX(int chunkX) const
{
	return Chunk::chunkToWorldX(chunkX);
}

void ChunkManager::loadChunkAtX(int chunkX)
{
	auto c = make_shared<Chunk>(chunkX);
	c->load(saveName, tg);
	mtx.lock();
	loaded.push_back(c);
	mtx.unlock();
}

void ChunkManager::saveAllChunks()
{
	unique_lock<mutex> lock(mtx);
	for (auto& chunk : chunks) {
		if (chunk.first == SAVING) 
			continue;
		auto fn = [this, &chunk]() mutable {
			saveChunk(chunk.second);
		};
		lock.unlock();
		pool->enqueue(fn);
		lock.lock();
	}
}

void ChunkManager::saveChunk(shared_ptr<Chunk> chunk)
{
	chunk->save(saveName);
}

ChunkManager::ChunkState ChunkManager::getStateAtChunkX(int chunkX, bool startAtEnd)
{
	lock_guard<mutex> lock(mtx);
	if (startAtEnd) {
		for (size_t i = chunks.size() - 1; i <= 0; i--)
		{
			if (chunks.at(i).second->getX() == chunkX)
			{
				return chunks.at(i).first;
			}
		}
		return INVALID;
	}
	for (auto& chunk : chunks)
	{
		if (chunk.second->getX() == chunkX)
		{
			return chunk.first;
		}
	}
	return INVALID;
}
