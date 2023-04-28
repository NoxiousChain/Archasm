//#pragma once
//#include "../../Common.hpp"
//#include <Node.hpp>
//#include <TileMap.hpp>
//#include <Dictionary.hpp>
//#include <String.hpp>
//#include <Thread.hpp>
//#include <Mutex.hpp>
//#include <Semaphore.hpp>
//#include <Array.hpp>
//
//#include <queue>
//#include <deque>
//#include <thread>
//#include <mutex>
//#include <condition_variable>
//#include <string>
//#include <cassert>
//#include <iostream>
//
//#include "Chunk.hpp"
//#include "Terrain/TerrainGenerator.hpp"
//
//// Mutex helper class. Automatically unlocks when it goes out of scope
//class MutexLock {
//	Ref<Mutex> mtx;
//
//public:
//	MutexLock(Ref<Mutex> m) : mtx{ m } { mtx->lock(); }
//	~MutexLock() { mtx->unlock(); }
//};
//
//// The main world scene will consist of several small tilemap scenes
//// that will be loaded in a separate thread as the game is running,
//// then added to the scene. The tilemaps will not be instantly removed
//// when going offscreen, but instead just set invisible temporarily until
//// the player moves decently far away. Thus, there should be around
//// 16-24 visible tilemaps + 8-16 invisible tilemaps at all times.
//
//class ThreadedChunkManager : public Node {
//	GODOT_CLASS(ThreadedChunkManager, Node)
//private:
//	// How many chunks are loaded but invisible on both sides of the deque
//	// This is basically a small cache to allow threads time to create new
//	// chunks to reduce stuttering
//	String saveName;		// Name of the currently loaded save file
//	deque<Chunk> tileMaps;	// Holds chunk data
//	TerrainGenerator* tg;	// Generates terrain
//
//	static const int MAX_THREADS = 8;
//
//	std::vector<Ref<Thread>> threads;
//	Ref<Mutex> threadMtx, queueMtx;
//
//public:
//	std::queue<size_t> availableThreads;
//
//public:
//	void initialize(const String& saveName, int playerX, int screenWidth);
//	static void _register_methods();
//	void _init();
//	void _ready();
//
//	// Sets saveName for hashing
//	void setSaveName(const String& saveName); 
//
//	// Converts world to chunk coordinates. 
//	int worldToChunkX(int worldX);
//	// Converts chunk to world coordinates. Returns x=0 relative to the chunk (left)
//	int chunkToWorldX(int chunkX);
//
//	void makeThreadAvailable(size_t index);
//	size_t getAvailableThread();
//
//	// LOADING:
//	// Loads/creates all chunks around the player
//	void loadAllChunks(int playerX, int screenW);
//	// Loads a single chunk at the given worldX and adds it to the back of the queue
//	// This should only be used once, then use loadChunk()
//	void loadChunkAtX(int worldX);
//	// Loads the next chunk right/left of the world
//	void loadChunk(bool right);
//	
//	// SAVING:
//	// Saves all chunks left in the deque
//	void saveAllChunks();
//	// Saves and removes the furthest left/right chunk in the world
//	void saveChunk(bool right);
//
//
//
//
//
//
//
//
//	/*
//	void loadChunk(bool right);
//	void saveChunk(bool right);
//	void loadCenterChunk(int x);
//
//	/// To be called when the world is loaded. Loads all chunks around the player.
//	/// @param playerX : player x coordinate
//	/// @param screenWidth : width of the screen in pixels
//	void loadAllChunks(int playerX, int screenWidth);
//	/// To be called when the world is saved. Saves all chunks left in the deque
//	void saveAllChunks();
//
//	// Finds next available thread for a function
//	int getAvailableThread();
//	void freeThread(int index);
//
//	// Thread safe function calls for loading and saving. Used by saveRightChunk() and saveLeftChunk()
//	//void threadSaveChunk(std::mutex& mtx, std::condition_variable& cv, bool& saveFinished, bool right);
//	//void threadLoadChunk(std::mutex& mtx, std::condition_variable& cv, bool& loadFinished, bool right);
//	void threadSaveChunk(bool right);
//	void threadLoadChunk(bool right);
//	*/
//};
