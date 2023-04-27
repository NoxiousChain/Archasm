#pragma once
#include "../../Common.hpp"
#include <Node.hpp>
#include <TileMap.hpp>
#include <Dictionary.hpp>
#include <String.hpp>
#include <Thread.hpp>
#include <Mutex.hpp>
#include <Semaphore.hpp>
#include <Array.hpp>

#include <deque>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <string>
#include <cassert>
#include <iostream>

#include "Chunk.hpp"
#include "Terrain/TerrainGenerator.hpp"

// The main world scene will consist of several small tilemap scenes
// that will be loaded in a separate thread as the game is running,
// then added to the scene. The tilemaps will not be instantly removed
// when going offscreen, but instead just set invisible temporarily until
// the player moves decently far away. Thus, there should be around
// 16-24 visible tilemaps + 8-16 invisible tilemaps at all times.

class ChunkManager : public Node {
	GODOT_CLASS(ChunkManager, Node)
private:
	// How many chunks are loaded but invisible on both sides of the deque
	// This is basically a small cache to allow threads time to create new
	// chunks to reduce stuttering
	String saveName;		// Name of the currently loaded save file
	deque<Chunk> tileMaps;	// Holds chunk data
	TerrainGenerator* tg;	// Generates terrain

	std::vector<Ref<Thread>> threads;
	Ref<Mutex> mtx;
	static const int MAX_THREADS = 8;
	int activeThreads = 0;

public:
	std::vector<bool> availableThreads;

public:
	static ChunkManager* create(const String& saveName, int playerX, int screenWidth);
	void initialize(const String& saveName, int playerX, int screenWidth);
	static void _register_methods();
	void _init();
	void _ready();

	// Sets saveName for hashing
	void setSaveName(const String& saveName);

	void loadChunk(bool right);
	void saveChunk(bool right);
	void loadCenterChunk(int x);

	/// To be called when the world is loaded. Loads all chunks around the player.
	/// @param playerX : player x coordinate
	/// @param screenWidth : width of the screen in pixels
	void loadAllChunks(int playerX, int screenWidth);
	/// To be called when the world is saved. Saves all chunks left in the deque
	void saveAllChunks();

	/// Converts world to chunk coordinates. 
	/// @param worldX : world x coordinate
	/// @returns chunk x coordinate
	int worldToChunkX(int worldX);
	/// Converts chunk to world coordinates
	/// @param chunkX : chunk x coordinate
	/// @returns worldX at (0,0) relative to the chunk
	int chunkToWorldX(int chunkX);

	// Finds next available thread for a function
	int getAvailableThread();
	void freeThread(int index);

	// Thread safe function calls for loading and saving. Used by saveRightChunk() and saveLeftChunk()
	//void threadSaveChunk(std::mutex& mtx, std::condition_variable& cv, bool& saveFinished, bool right);
	//void threadLoadChunk(std::mutex& mtx, std::condition_variable& cv, bool& loadFinished, bool right);
	void threadSaveChunk(bool right);
	void threadLoadChunk(bool right);
};