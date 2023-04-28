#pragma once

#include "../../../Common.hpp"

#include <time.h>
#include <cmath>

#include <Node.hpp>
#include <Ref.hpp>
#include <OpenSimplexNoise.hpp>
#include <TileMap.hpp>
#include <File.hpp>
#include <JSON.hpp>
#include <JSONParseResult.hpp>
#include <Array.hpp>
#include <RandomNumberGenerator.hpp>

#include "../Blocks/BlockType.hpp"
#include "../../KDTree/KDTree.hpp"
#include "../ChunkConstants.hpp"

/*
Terrain generator. This class handles all functionality for generating the world.

Textures and generation data are read from a json file at runtime and stored in memory,
which makes it very easy to add new blocks, modify generation parameters, and read generation data.

*/

class TerrainGenerator : public Node {
	GODOT_CLASS(TerrainGenerator, Node)

private:
	// Noise maps
	Ref<OpenSimplexNoise> biomeNoise[3];	// elevation, humidity, temperature
	Ref<OpenSimplexNoise> heightNoise;		// for generating height map
	Ref<OpenSimplexNoise> caveNoise;
	
	// Tile data
	KDTree blockTypes;

public:
	~TerrainGenerator();
	static void _register_methods();
	void _init();
	void _ready();

	void generateChunk(int chunkX, TileMap* tileMap);

private:
	void loadBlockDataFromJSON(const String& filepath);

	int getBlockTypeFromNoise(float temperature, float humidity, float height);

	// Helper functions
	// Maps v from the range [ol, oh] to the range [nl, nh]
	float mapv(float v, float ol, float oh, float nl, float nh);
	// Returns the intermediate point between a and b, which is mu factor away from a
	float cosp(float a, float b, float mu);

	// Generation functions. These return normalized values [0-1]
	std::vector<float> getElevationNoise(int chunkX);
	std::vector<float> getHumidityNoise(int chunkX);
	std::vector<float> getTemperatureNoise(int chunkX);
	std::vector<float> getHeightNoise(int chunkX);

	// HEIGHT MAP GENERATION --------------------------------------------------//
	// Generates a superposition sampled, cosinely interpolated terrain on a chunk. 
	std::vector<float> terrainSuperposCosp(int chunkX, int iterations = 8);
	// ------------------------------------------------------------------------//
};

