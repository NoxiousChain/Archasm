#pragma once
#include "../../Common.hpp"
#include <Node2D.hpp>
#include <TileMap.hpp>
#include <Dictionary.hpp>
#include <String.hpp>
#include <Array.hpp>
#include <TileSet.hpp>

#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <cassert>
#include <iostream>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <future>
#include <fstream>

#include "Chunk.hpp"
#include "Terrain/TerrainGenerator.hpp"
#include "../Helpers/ThreadPool/ThreadPool.hpp"

#define CATEGORY_SEPARATOR String("")

class ChunkManager : public Node2D {
	GODOT_CLASS(ChunkManager, Node2D)

private: // Constants
	// SIGNALS
	String SIGNAL_TOGGLE_INTERACT = String("toggle_interact");

	// THREADING
	String THREADING_PARAMETERS;		// category
	size_t THREAD_EXP;

	// GENERATION PARAMETERS
	String CAVE_GENERATION_PARAMETERS;	// category
	float CAVE_CHANCE_TO_START_ALIVE;
	float CAVE_MAX_HEIGHT_WEIGHT;
	float CAVE_THRESHOLD;
	float CAVE_THRESHOLD_Y_MODIFIER;

private: // Multithreading
	ThreadPool* pool;
	mutex mtx; // for read/write on chunks
	condition_variable cv;

private: // Data
	String saveName;
	unordered_map<int, shared_ptr<Chunk>> loadedChunks; // index -> chunk
	unordered_set<int> queuedChunks, queuedForSave, queuedForDeletion;
	unordered_map<int, future<chunkdata_t>> loadingChunks;

	//deque<shared_ptr<Chunk>> chunks;
	TerrainGenerator* tg;

	Ref<PackedScene> tileMapScene;

public:
	~ChunkManager();
	static void _register_methods();
	void _init();
	void _ready();
	void initialize(String saveName, int playerX, int screenW);
	void update();

	void setSaveName(String saveName);

	int worldToChunkX(int worldX);
	int chunkToWorldX(int chunkX);

	int getNumChunks();

	void tryLoadChunk(int chunkX);
	void loadAllChunks(int playerX, int screenW);
	chunkdata_t loadChunkAtX(int chunkX);

	// Not sure how I want to construct chunk data yet, so I have both options here
	void applyChunkData(int chunkX, const chunkdata_t& data);

	// Saves chunks; does not delete them
	void saveAllChunks();
	// Assumes that the chunk exists and has been loaded already
	void saveChunkAtX(int chunkX);
	// Wrapper function to save chunk
	void saveChunk(shared_ptr<Chunk> chunk);

	// Handles actual deletion of chunks. 
	// This function is not threadsafe in itself. MUST be called within a lock_guard block
	// Returns whether or not the chunk has been deleted
	bool deleteChunkAtX(int chunkX);
	void deleteAllChunks();
};
