#pragma once
#include "KDNode.hpp"

// Implements a KD tree specifically designed to handle BlockType objects
// This is used for terrain generation - stores block data via three parameters:
// elevation, humidity, and temperature. Returns closest average match to all three

class KDTree {
private:
	std::unique_ptr<KDNode> root;
public:
	KDTree();

	void insert(const BlockType& block);
	BlockType nearestNeighbor(double elevation, double humidity, double temperature);

private:
	std::unique_ptr<KDNode> insertRecursive(std::unique_ptr<KDNode>, const BlockType& block, int depth);
	BlockType nearestRecursive(KDNode* node, double elevation, double humidity, double temperature, int depth);
	double distanceSquared(const BlockType& block, double elevation, double humidity, double temperature);
};