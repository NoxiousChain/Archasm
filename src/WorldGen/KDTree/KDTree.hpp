#pragma once
#include "KDNode.hpp"
#include <cmath>

// Implements a KD tree specifically designed to handle BlockType objects
// This is used for terrain generation - stores block data via three parameters:
// elevation, humidity, and temperature. Returns closest average match to all three

class KDTree {
private:
	KDNode* root;
public:
	KDTree();
	~KDTree(); // Automatically frees memory when out of scope

	void insert(const BlockType& block);
	BlockType nearestNeighbor(double elevation, double humidity, double temperature);

private:
	KDNode* insertRecursive(KDNode*, const BlockType& block, int depth);
	BlockType nearestRecursive(KDNode* node, double elevation, double humidity, double temperature, int depth);
	double distanceSquared(const BlockType& block, double elevation, double humidity, double temperature);

	// Recursively frees memory
	void deleteNodes(KDNode* node);
};