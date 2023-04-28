#include "ChunkManager.hpp"

void ChunkManager::_register_methods()
{
	register_method("_ready", &ChunkManager::_ready);
	register_method("initialize", &ChunkManager::initialize);
	register_method("set_save_name", &ChunkManager::setSaveName);
	register_method("world_to_chunk_x", &ChunkManager::worldToChunkX);
	register_method("chunk_to_world_x", &ChunkManager::chunkToWorldX);
	register_method("load_all_chunks", &ChunkManager::loadAllChunks);
	register_method("load_chunk", &ChunkManager::loadChunk);
	register_method("load_chunk_at_x", &ChunkManager::loadChunkAtX);
	register_method("save_all_chunks", &ChunkManager::saveAllChunks);
	register_method("save_chunk", &ChunkManager::saveChunk);
	register_method("delete_chunk", &ChunkManager::deleteChunk);
	register_method("delete_all_chunks", &ChunkManager::deleteAllChunks);
}

void ChunkManager::_init()
{
	Godot::print("ChunkManager::_init()");
}

void ChunkManager::_ready()
{
	Godot::print("ChunkManager::_ready()");
	tg = TerrainGenerator::_new();
	tg->_ready(); // _ready() is never called automatically for some reason, so I do it manually here
}

void ChunkManager::initialize(const String& saveName, int playerX, int screenW)
{
	Godot::print("ChunkManager::initialize()");
	this->saveName = saveName;
	loadAllChunks(playerX, screenW);
}

void ChunkManager::setSaveName(const String& saveName)
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

void ChunkManager::loadAllChunks(int playerX, int screenW)
{
	int chunkX = Chunk::worldToChunkX(playerX);
	// worldToChunkX(screenW) + 1 returns the number of chunks visible on the screen
	int numChunks = (Chunk::worldToChunkX(screenW) + 1) + 2 * HIDDEN_CHUNKS;
	//Godot::print("playerX: " + String::num_int64(playerX));
	//Godot::print("screenW: " + String::num_int64(screenW));
	//Godot::print("chunkX: " + String::num_int64(chunkX));
	//Godot::print("numChunks: " + String::num_int64(numChunks));
	//Godot::print("int i: " + String::num_int64(chunkX - (numChunks / 2 + 1)));
	//Godot::print("i < : " + String::num_int64(numChunks / 2 + 1));
	// [-3, 5]
	for (int i = chunkX - (numChunks / 2 + 1); i <= numChunks / 2 + 1; i++) {
		loadChunkAtX(i);
	}
	
	// Hide invisible chunks
	//for (size_t i = 0; i < HIDDEN_CHUNKS; i++) {
	//	chunks[i]->getTileMap()->hide();
	//	chunks[chunks.size() - (i + 1)]->getTileMap()->hide();
	//}
}

void ChunkManager::loadChunkAtX(int chunkX)
{
	auto c = make_shared<Chunk>(chunkX);
	c->load(saveName, tg);
	add_child(c->getTileMap());
	c->getTileMap()->set_owner(this);
	c->getTileMap()->set_position(Vector2(real_t(chunkToWorldX(chunkX)), 0.f));
	chunks.push_back(c);
}

void ChunkManager::loadChunk(bool right)
{
	if (right) {
		int chunkX = chunks.back()->getX() + 1;
		auto c = make_shared<Chunk>(chunkX);
		add_child(c->getTileMap());
		c->getTileMap()->set_owner(this);
		c->load(saveName, tg);
		c->getTileMap()->set_position(Vector2(real_t(chunkToWorldX(chunkX)), 0.f));
		chunks.push_back(c);
	}
	else {
		int chunkX = chunks.back()->getX() - 1;
		auto c = make_shared<Chunk>(chunkX);
		add_child(c->getTileMap());
		c->getTileMap()->set_owner(this);
		c->load(saveName, tg);
		c->getTileMap()->set_position(Vector2(real_t(chunkToWorldX(chunkX)), 0.f));
		chunks.push_front(c);
	}
}

void ChunkManager::saveAllChunks()
{
	for (auto& chunk : chunks) {
		chunk->save(saveName);
	}
}

void ChunkManager::saveChunk(bool right)
{
	right ? chunks.back()->save(saveName) : chunks.front()->save(saveName);
}

void ChunkManager::deleteChunk(bool right, bool updateVisibility)
{
	Godot::print("deleting chunk...");
	if (right) {
		remove_child(chunks.back()->getTileMap());
		chunks.pop_back();
		//if (updateVisibility) {
		//	chunks[chunks.size() - (HIDDEN_CHUNKS + 1)]->getTileMap()->set_visible(false);
		//	chunks[HIDDEN_CHUNKS - 1]->getTileMap()->set_visible(true);
		//}
	}
	else {
		remove_child(chunks.front()->getTileMap());
		chunks.pop_front();
		//if (updateVisibility) {
		//	chunks[HIDDEN_CHUNKS - 1]->getTileMap()->set_visible(false);
		//	chunks[chunks.size() - (HIDDEN_CHUNKS + 1)]->getTileMap()->set_visible(true);
		//}
	}
}

void ChunkManager::deleteAllChunks()
{
	for (auto& chunk : chunks) {
		remove_child(chunk->getTileMap());
		// Chunks handle their own tilemap deletion
	}
	chunks.clear();
}
