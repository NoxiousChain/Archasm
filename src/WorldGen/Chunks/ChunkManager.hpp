#pragma once
#include "../../Common.hpp"
#include <Node.hpp>
#include <TileMap.hpp>
#include <Dictionary.hpp>
#include <String.hpp>
#include <Array.hpp>

#include <deque>
#include <string>
#include <cassert>
#include <iostream>

#include "Chunk.hpp"
#include "Terrain/TerrainGenerator.hpp"

class ChunkManager : public Node {
	GODOT_CLASS(ChunkManager, Node)

private:
	String saveName;
	deque<Chunk> chunks;
	TerrainGenerator* tg;

public:
	static void _register_methods();
	void _init();
	void _ready();
	void initialize(const String& saveName, int playerX, int screenW);

	void setSaveName(const String& saveName);

	int worldToChunkX(int worldX);
	int chunkToWorldX(int chunkX);

	void loadAllChunks(int playerX, int screenW);
	void loadChunkAtX(int chunkX); // not overloaded intentionally
	void loadChunk(bool right);

	// Saves chunks. Does not delete them off the ends
	void saveAllChunks();
	void saveChunk(bool right);

	// Handles actual deletion of chunks. 
	void deleteChunk(bool right, bool updateVisibility = true);
	void deleteAllChunks();
};