#include "KDTree.hpp"

KDTree::KDTree() : root{ nullptr } 
{
}

KDTree::~KDTree()
{
	deleteNodes(root);
}

void KDTree::insert(Biome biome)
{
	root = insertRecursive(root, biome, 0);
}

// Performs a recursive insert that searches for the best place in the tree to place a new node
// Returns the final calculated root node
KDNode* KDTree::insertRecursive(KDNode* node, Biome biome, int depth)
{
	if (!node) {
		return new KDNode(biome, depth % 3);
	}
	
	int axis = node->axis;
	Biome::Conditions nodeConditions = node->biome.getConditions();
	Biome::Conditions biomeConditions = biome.getConditions();
	
	double nodeKey = axis == 0 ? nodeConditions.elevation.mid() : (axis == 1 ? nodeConditions.humidity.mid() : nodeConditions.temperature.mid());
	double biomeKey = axis == 0 ? biomeConditions.elevation.mid() : (axis == 1 ? biomeConditions.humidity.mid() : biomeConditions.temperature.mid());
	
	if (biomeKey < nodeKey) {
		node->left = insertRecursive(node->left, biome, depth + 1);
	}
	else {
		node->right = insertRecursive(node->right, biome, depth + 1);
	}
	return node;
}


Biome KDTree::nearestNeighbor(double elevation, double humidity, double temperature)
{
	return nearestRecursive(root, elevation, humidity, temperature, 0)->biome;
}

KDNode* KDTree::nearestRecursive(KDNode* node, double elevation, double humidity, double temperature, int depth)
{
	if (!node) {
		return nullptr; // Return nullptr if none found
	}

	int axis = node->axis;
	const Biome::Conditions& nodeConditions = node->biome.getConditions();

	double nodeKey = axis == 0 ? nodeConditions.elevation.mid() : (axis == 1 ? nodeConditions.humidity.mid() : nodeConditions.temperature.mid());
	double targetKey = axis == 0 ? elevation : (axis == 1 ? humidity : temperature);

	KDNode* next = targetKey < nodeKey ? node->left : node->right;
	KDNode* other = targetKey < nodeKey ? node->right : node->left;

	KDNode* best = nearestRecursive(next, elevation, humidity, temperature, depth + 1);
	double bestDistSq = best ? distanceSquared(&best->biome, elevation, humidity, temperature) : std::numeric_limits<double>::max(); // if doesn't exist, set distance to max

	// Check if current node is better than the best match so far
	double currentDistSq = distanceSquared(&node->biome, elevation, humidity, temperature);
	if (!best || bestDistSq > currentDistSq) {
		// Could also clamp values and verify that all values are within ranges
		// However this provides a slightly more realistic scenerio where values aren't exact
		best = node;
		bestDistSq = currentDistSq;
	}

	// Calculate the distance for this particular plane
	double planeDistance = targetKey - nodeKey;
	// If the distance^2 is less than the current best distance squared, we check the other node
	if (planeDistance * planeDistance < bestDistSq) {
		KDNode* tempBest = nearestRecursive(other, elevation, humidity, temperature, depth + 1);
		// If the other node is better, we switch nodes
		if (tempBest && distanceSquared(&tempBest->biome, elevation, humidity, temperature) < bestDistSq) {
			best = tempBest;
		}
	}

	return best;
}

double KDTree::distanceSquared(Biome* biome, double elevation, double humidity, double temperature) const
{
	Biome::Conditions biomeConditions = biome->getConditions();
	// Get distance from actual (midpoint) to each value
	double deltaElevation = biomeConditions.elevation.mid() - elevation;
	double deltahumidity = biomeConditions.humidity.mid() - humidity;
	double deltaTemp = biomeConditions.temperature.mid() - temperature;
	
	// 3D pythagorean theorem squared
	return deltaElevation * deltaElevation + deltahumidity * deltahumidity + deltaTemp * deltaTemp;
}

void KDTree::deleteNodes(KDNode* node)
{
	if (node) {
		deleteNodes(node->left);
		deleteNodes(node->right);
		delete node;
	}
}

void KDTree::printToFile(KDNode* node, std::ofstream& file, std::string indent)
{
	if (!node) return;

	printToFile(node->left, file, indent + "\t");

	Biome::Conditions biomeConditions = node->biome.getConditions();
	file << indent << "Biome: " << node->biome.getName().alloc_c_string() \
		<< ", Elevation: " << biomeConditions.elevation.min \
		<< ", Humidity: " << biomeConditions.humidity.min \
		<< ", Temperature: " << biomeConditions.temperature.min \
		<< ", Axis: " << node->axis \
		<< std::endl;

	printToFile(node->right, file, indent + "\t");
}
