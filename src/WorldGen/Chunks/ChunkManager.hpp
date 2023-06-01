#pragma once
#include "../../Common.hpp"
#include <Node2D.hpp>
#include <TileMap.hpp>
#include <Dictionary.hpp>
#include <String.hpp>
#include <Array.hpp>
#include <TileSet.hpp>
#include <PackedScene.hpp>

#include <deque>
#include <string>
#include <cassert>
#include <iostream>
#include <memory>
#include <map>

#include "Chunk.hpp"
#include "Terrain/TerrainGenerator.hpp"
#include "../Helpers/ThreadPool/ThreadPool.hpp"

#define CATEGORY_SEPARATOR String("------------------------------")

class ChunkManager : public Node2D {
	GODOT_CLASS(ChunkManager, Node2D)
private:
	enum ChunkState {
		LOADING = 0,
		LOADED = 1,
		SAVING = 2,
		INVALID = -1
	};

private:
	// SIGNALS
	String SIGNAL_TOGGLE_INTERACT = String("toggle_interact");

	// GENERATION PARAMETERS

	// Threading
	String THREADING_PARAMETERS;		// category
	size_t THREAD_EXP;

	// Cave
	String CAVE_GENERATION_PARAMETERS;	// category
	float CAVE_CHANCE_TO_START_ALIVE;
	float CAVE_MAX_HEIGHT_WEIGHT;
	float CAVE_THRESHOLD;
	float CAVE_THRESHOLD_Y_MODIFIER;

private:
	String saveName;
	// 0 = loading 1 = saving 2 = loaded
	deque<pair<ChunkState, shared_ptr<Chunk>>> chunks;
	vector<shared_ptr<Chunk>> loaded;
	TerrainGenerator* tg;

	Ref<PackedScene> tileMapScene;

private:
	unique_ptr<ThreadPool> pool;
	mutex mtx;

public:
	~ChunkManager();
	static void _register_methods();
	void _init();
	void _ready();
	void initialize(String saveName, int playerX, int screenW);
	void update(int playerX, int screenW);

	void setSaveName(String saveName);

	int worldToChunkX(int worldX) const;
	int chunkToWorldX(int chunkX) const;

	void loadChunkAtX(int chunkX);

	// Saves chunks. Does not delete them off the ends
	void saveAllChunks();
	void saveChunk(shared_ptr<Chunk> chunk);

private:
	ChunkState getStateAtChunkX(int chunkX, bool startAtEnd = false);
};
