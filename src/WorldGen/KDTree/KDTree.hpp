#pragma once
#include "KDNode.hpp"
#include <cmath>

// Implements a KD tree specifically designed to handle Biome objects
// This is used for terrain generation - stores biome data via three parameters:
// elevation, humidity, and temperature. Returns closest average match to all three

class KDTree {
private:
	KDNode* root;
public:
	KDTree();
	~KDTree(); // Automatically frees memory when out of scope

	void insert(const Biome& biome);
	Biome nearestNeighbor(double elevation, double humidity, double temperature);

private:
	KDNode* insertRecursive(KDNode*, const Biome& biome, int depth);
	Biome nearestRecursive(KDNode* node, double elevation, double humidity, double temperature, int depth);
	double distanceSquared(const Biome& biome, double elevation, double humidity, double temperature);

	// Recursively frees memory
	void deleteNodes(KDNode* node);
};