#include "KDTree.hpp"
#include <limits>
#include <cmath>

KDTree::KDTree() : root{ nullptr } {}

// Wrapper for insertRecursive function
void KDTree::insert(const BlockType& block)
{
	root = insertRecursive(std::move(root), block, 0);
}

// Performs a recursive insert that searches for the best place in the tree to place a new node
// Returns the final calculated root node
std::unique_ptr<KDNode> KDTree::insertRecursive(std::unique_ptr<KDNode> node, const BlockType& block, int depth)
{
	if (!node) {
		return std::make_unique<KDNode>(block, depth % 3);
	}

	int axis = node->axis;
	const BlockType::Conditions& nodeConditions = node->block.getConditions();
	const BlockType::Conditions& blockConditions = block.getConditions();

	double nodeKey = axis == 0 ? nodeConditions.elevation.min : (axis == 1 ? nodeConditions.
		.min : nodeConditions.temperature.min);
	double blockKey = axis == 0 ? blockConditions.elevation.min : (axis == 1 ? blockConditions.humidity.min : blockConditions.temperature.min);

	if (blockKey < nodeKey) {
		node->left = insertRecursive(std::move(node->left), block, depth + 1);
	}
	else {
		node->right = insertRecursive(std::move(node->right), block, depth + 1);
	}

	return node;
}

// Wrapper for nearestRecursive function
// Finds the best matching BlockType for the specified conditions
BlockType KDTree::nearestNeighbor(double elevation, double humidity, double temperature) {
	return nearestRecursive(root.get(), elevation, humidity, temperature, 0);
}

// Performs a recursive search to find the node that best matches the parameters
BlockType KDTree::nearestRecursive(KDNode* node, double elevation, double humidity, double temperature, int depth)
{
	if (!node) {
		return BlockType(); // Return empty block if none found
	}

	int axis = node->axis;
	const BlockType::Conditions& nodeConditions = node->block.getConditions();
	
	double nodeKey = axis == 0 ? nodeConditions.elevation.min : (axis == 1 ? nodeConditions.humidity.min : nodeConditions.temperature.min);
	double targetKey = axis == 0 ? elevation : (axis == 1 ? humidity : temperature);

	KDNode* next = targetKey < nodeKey ? node->left.get() : node->right.get();
	KDNode* other = targetKey < nodeKey ? node->right.get() : node->left.get();

	BlockType best = nearestRecursive(next, elevation, humidity, temperature, depth + 1);
	double bestDistSq = distanceSquared(best, elevation, humidity, temperature);

	if (bestDistSq > distanceSquared(node->block, elevation, humidity, temperature)) {
		best = node->block;
	}

	// Calculate the distance for this particular plane
	double planeDistance = targetKey - nodeKey;
	// If the distance^2 is less than the current best distance squared, we check the other node
	if (planeDistance * planeDistance < bestDistSq) {
		BlockType tempBest = nearestRecursive(other, elevation, humidity, temperature, depth + 1);
		// If the other node is better, we switch nodes
		if (distanceSquared(tempBest, elevation, humidity, temperature) < bestDistSq) {
			best = tempBest;
		}
	}

	return best;
}

double KDTree::distanceSquared(const BlockType& block, double elevation, double humidity, double temperature)
{
	const BlockType::Conditions& blockConditions = block.getConditions;
	// Get distance from actual to minimum for each
	double deltaElevation = blockConditions.elevation.min - elevation;
	double deltahumidity = blockConditions.humidity.min - humidity;
	double deltaTemp = blockConditions.temperature.min - temperature;
	
	// 3D pythagorean theorem squared
	return deltaElevation * deltaElevation + deltahumidity * deltahumidity + deltaTemp * deltaTemp;
}