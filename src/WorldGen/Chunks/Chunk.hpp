#pragma once
#include "../../Common.hpp"
#include <vector>
#include <TileMap.hpp>
#include <File.hpp>
#include <HashingContext.hpp>
#include <TileSet.hpp>
#include <ResourceLoader.hpp>
#include <PackedScene.hpp>
#include "Terrain/TerrainGenerator.hpp"
#include "ChunkConstants.hpp"

// Class that contains all data for a chunk, as well as loading and saving functionality.
// May need to add support for things that aren't just tiles (e.g. treasure chests, mobs, etc),
// Although those could save themselves just based on position and load when the chunk comes into view
class Chunk {
private:
	int cx;	// Because chunks span the entire world vertically, it just needs an x coordinate
	TileMap* tileMap;

public:
	Chunk(int x);
	~Chunk();

	void instance(Ref<PackedScene> tileMapScene);

	chunkdata_t load(String saveName, TerrainGenerator* tg);
	void save(String saveName);

	int getX() const;
	TileMap* getTileMap() const;

	/// Converts world to chunk coordinates. 
	/// @param worldX : world x coordinate
	/// @returns chunk x coordinate
	static int worldToChunkX(int worldX);
	/// Converts chunk to world coordinates
	/// @param chunkX : chunk x coordinate
	/// @returns worldX at (0,0) relative to the chunk
	static int chunkToWorldX(int chunkX);

private:
	String hashName(String saveName);
};

