#pragma once

#include "../../../Common.hpp"

#include <time.h>
#include <cmath>
#include <cassert>
#include <tuple>
#include <unordered_map>
#include <memory>
#include <bitset>
#include <mutex>

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
	// Pointer to parent ChunkManager. This is to be used solely for interfacing with Godot
	// to adjust generation parameters through the editor.
	Node2D* cm = nullptr;

	// Noise maps
	Ref<OpenSimplexNoise> biomeNoise[4];	// elevation, humidity, temperature, elev. modifier
	Ref<OpenSimplexNoise> caveNoise;
	mutex noiseMtx;

	// Using a RRange as an RNG wrapper
	RRange<float> rand01;
	
	// Block data - map from name to block. This is a dictionary because unordered_map doesn't support godot strings as keys
	Dictionary blockMap;
	// Index to block
	unordered_map<size_t, Block*> blockIndexMap;
	// Chunk data
	KDTree biomes;
	mutex kdMtx;

public:
	~TerrainGenerator();
	static void _register_methods();
	void _init();
	void _ready();

	void setParent(Node2D* cm);

	void generateChunk(int chunkX, TileMap* tileMap);

private:
	void loadBlockDataFromJSON(const String& filepath);
	void loadBiomeDataFromJSON(const String& filepath);


	// ------------------------------------------------------------------------//
	// Generates height, temperature, and humidity values for a chunk
	// Temperature & humidity values are adjusted based on elevation to simulate
	// realistic environments (higher elevation -> colder & drier)
	std::vector<std::tuple<float, float, float>> generateChunkData(int chunkX) const;

private: // Helper functions

	// Mapping/interpolation

	// Maps v from the range [ol, oh] to the range [nl, nh]
	float mapv(float v, float ol, float oh, float nl, float nh);
	// Maps noise value from [-1, 1] to [0, 1]
	float nnoise(float in) const;
	// Returns the intermediate point between a and b, which is mu factor away from a
	float cosp(float a, float b, float mu);

	// Cave generation
	bool isSolid(int x, int y, int maxHeight);
	float getCaveNoise(int x, int y);

};