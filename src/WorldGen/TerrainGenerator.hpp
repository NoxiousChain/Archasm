#pragma once

#include "../Common.hpp"
#include <time.h>
#include <Node.hpp>
#include <Ref.hpp>
#include <OpenSimplexNoise.hpp>
#include <TileMap.hpp>
#include <File.hpp>
#include <JSON.hpp>
#include <JSONParseResult.hpp>
#include <Array.hpp>
#include <RandomNumberGenerator.hpp>
#include "BlockType.hpp"
#include "KDTree/KDTree.hpp"

/*
Terrain generator. This class handles all functionality for generating the world.

Textures and generation data are read from a json file at runtime and stored in memory,
which makes it very easy to add new blocks, modify generation parameters, and read generation data.

*/

class TerrainGenerator : public Node {
	GODOT_CLASS(TerrainGenerator, Node)

private:
	// RNG
	Ref<RandomNumberGenerator> rng;

	// Noise maps
	Ref<OpenSimplexNoise> biomeNoise[3];	// elevation, wetness, temperature
	Ref<OpenSimplexNoise> heightNoise;		// for generating height map
	Ref<OpenSimplexNoise> caveNoise;

	// Tile map
	TileMap* tilemap;
	
	// Tile data
	KDTree blockTypes;

public:
	static void _register_methods();
	void _init();
	void _ready();

private:
	void loadBlockDataFromJSON(const String& filepath);
};