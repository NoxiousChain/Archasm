#pragma once
#include "KDNode.hpp"

// Implements a K-D tree specifically designed to handle BlockType objects
// This is used for terrain generation - stores block data by three parameters:
// elevation, wetness, and temperature. Returns closest match

class KDTree {
private:
	std::unique_ptr<KDNode> root;
public:
	KDTree();

	void insert(const BlockType& block);
	BlockType nearestNeighbor(double elevation, double wetness, double temperature);

private:
	std::unique_ptr<KDNode> insertRecursive(std::unique_ptr<KDNode>, const BlockType& block, int depth);
	BlockType nearestRecursive(KDNode* node, double elevation, double wetness, double temperature, int depth);
	double distanceSquared(const BlockType& block, double elevation, double wetness, double temperature);
};