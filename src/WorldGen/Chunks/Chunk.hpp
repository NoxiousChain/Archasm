#pragma once
#include "../../Common.hpp"
#include <vector>
#include <TileMap.hpp>
#include <File.hpp>
#include <HashingContext.hpp>

// Class that contains all data for a chunk, as well as loading and saving functionality.
// May need to add support for things that aren't just tiles (e.g. treasure chests, mobs, etc),
// Although those could save themselves just based on position and load when the chunk comes into view
class Chunk {
public:
	// Chunk and cell size constants
	static const size_t CHUNK_WIDTH = 32;
	static const size_t CHUNK_HEIGHT = 256;
	static const size_t CELL_SIZE = 16;

private:
	int cx;	// Because chunks span the entire world vertically, it just needs an x coordinate
	Ref<TileMap> tileMap;

public:
	// This constructor attempts to load the chunk immediately
	Chunk(int x, const String& saveName);
	Chunk(int x);

	void load(const String& saveName);
	void save(const String& saveName);

	int getX() const;
	Ref<TileMap> getTileMap() const;

private:
	String hashName(const String& saveName);
};