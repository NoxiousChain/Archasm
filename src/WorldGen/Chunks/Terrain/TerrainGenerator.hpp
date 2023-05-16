#pragma once

#include "../../../Common.hpp"

#include <time.h>
#include <cmath>
#include <cassert>
#include <tuple>
#include <unordered_map>
#include <memory>

#include <Node.hpp>
#include <Ref.hpp>
#include <OpenSimplexNoise.hpp>
#include <TileMap.hpp>
#include <File.hpp>
#include <JSON.hpp>
#include <JSONParseResult.hpp>
#include <Array.hpp>
#include <RandomNumberGenerator.hpp>
#include <Script.hpp>
#include <Dictionary.hpp>

#include "../Biome/Block.hpp"
#include "../Biome/Biome.hpp"
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
	Ref<OpenSimplexNoise> biomeNoise[4];	// elevation, humidity, temperature, elev. modifier
	Ref<OpenSimplexNoise> caveNoise;
	
	// Block data - map from name to block. This is a dictionary because unordered_map doesn't support godot strings as keys
	Dictionary blockMap;
	//unordered_map<String, Block*> blockMap;
	// Index to block
	unordered_map<size_t, Block*> blockIndexMap;
	// Chunk data
	KDTree chunks;

public:
	~TerrainGenerator();
	static void _register_methods();
	void _init();
	void _ready();

	void generateChunk(int chunkX, TileMap* tileMap);

private:
	void loadBlockDataFromJSON(const String& filepath);
	void loadBiomeDataFromJSON(const String& filepath);

	// Helper functions
	
	// Maps v from the range [ol, oh] to the range [nl, nh]
	float mapv(float v, float ol, float oh, float nl, float nh);
	// Maps noise value from [-1, 1] to [0, 1]
	float nnoise(float in) const;
	// Returns the intermediate point between a and b, which is mu factor away from a
	float cosp(float a, float b, float mu);


	// ------------------------------------------------------------------------//
	// Generates height, temperature, and humidity values for a chunk
	// Temperature & humidity values are adjusted based on elevation to simulate
	// realistic environments (higher elevation -> colder & drier)
	std::vector<std::tuple<float, float, float>> generateChunkData(int chunkX) const;

};