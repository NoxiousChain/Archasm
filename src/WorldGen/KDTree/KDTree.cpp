#include "KDTree.hpp"

KDTree::KDTree() : root{ nullptr } 
{
}

KDTree::~KDTree()
{
	deleteNodes(root);
}

void KDTree::insert(Biome* biome)
{
	lock_guard<mutex> lock(mtx);
	std::ofstream logFile;
	logFile.open("C:\\Users\\Forest\\Desktop\\log.txt", std::ios_base::app);
	logFile << "root currently at: " << root << std::endl;
	root = insertRecursive(root, biome, 0);
	logFile << "after insertRecursive root now at : " << root << std::endl;
	printToFile(root, logFile);
	logFile.close();
}

// Performs a recursive insert that searches for the best place in the tree to place a new node
// Returns the final calculated root node
KDNode* KDTree::insertRecursive(KDNode* node, Biome* biome, int depth)
{
	std::ofstream logFile("C:\\Users\\Forest\\Desktop\\log.txt", std::ios::app);
	logFile << "Insert recursive called with depth: " << depth << std::endl;

	if (!node) {
		logFile << "Creating new node at depth: " << depth << std::endl;
		logFile.close();
		return new KDNode(biome, depth % 3);
	}
	
	int axis = node->axis;
	Biome::Conditions nodeConditions = node->biome->getConditions();
	Biome::Conditions biomeConditions = biome->getConditions();
	
	double nodeKey = axis == 0 ? nodeConditions.elevation.min : (axis == 1 ? nodeConditions.humidity.min : nodeConditions.temperature.min);
	double biomeKey = axis == 0 ? biomeConditions.elevation.min : (axis == 1 ? biomeConditions.humidity.min : biomeConditions.temperature.min);
	
	if (biomeKey < nodeKey) {
		logFile << "Inserting on the left at depth: " << depth << std::endl;
		node->left = insertRecursive(node->left, biome, depth + 1);
		logFile << "New node at: " << node->left << std::endl;
	}
	else {
		logFile << "Inserting on the right at depth: " << depth << std::endl;
		logFile.close();
		node->right = insertRecursive(node->right, biome, depth + 1);
	}
	logFile << "Returning node: " << node << std::endl;
	logFile << "Returning biome: " << node->biome->toString().alloc_c_string() << std::endl;
	logFile.close();
	return node;
}


Biome* KDTree::nearestNeighbor(double elevation, double humidity, double temperature)
{
	lock_guard<mutex> lock(mtx);
	return nearestRecursive(root, elevation, humidity, temperature, 0);
}

Biome* KDTree::nearestRecursive(KDNode* node, double elevation, double humidity, double temperature, int depth)
{
	if (!node) {
		return nullptr; // Return nullptr if none found
	}

	int axis = node->axis;
	const Biome::Conditions& nodeConditions = node->biome->getConditions();

	double nodeKey = axis == 0 ? nodeConditions.elevation.min : (axis == 1 ? nodeConditions.humidity.min : nodeConditions.temperature.min);
	double targetKey = axis == 0 ? elevation : (axis == 1 ? humidity : temperature);

	KDNode* next = targetKey < nodeKey ? node->left : node->right;
	KDNode* other = targetKey < nodeKey ? node->right : node->left;

	Biome* best = nearestRecursive(next, elevation, humidity, temperature, depth + 1);
	double bestDistSq = best ? distanceSquared(best, elevation, humidity, temperature) : std::numeric_limits<double>::max(); // if doesn't exist, set distance to max

	// Check if current node is better than the best match so far
	double currentDistSq = distanceSquared(node->biome, elevation, humidity, temperature);
	if (!best || bestDistSq > currentDistSq) {
		best = node->biome;
		bestDistSq = currentDistSq;
	}

	// Calculate the distance for this particular plane
	double planeDistance = targetKey - nodeKey;
	// If the distance^2 is less than the current best distance squared, we check the other node
	if (planeDistance * planeDistance < bestDistSq) {
		Biome* tempBest = nearestRecursive(other, elevation, humidity, temperature, depth + 1);
		// If the other node is better, we switch nodes
		if (tempBest && distanceSquared(tempBest, elevation, humidity, temperature) < bestDistSq) {
			best = tempBest;
		}
	}

	return best;
}

double KDTree::distanceSquared(Biome* biome, double elevation, double humidity, double temperature)
{
	Biome::Conditions biomeConditions = biome->getConditions();
	// Get distance from actual to minimum for each
	double deltaElevation = biomeConditions.elevation.min - elevation;
	double deltahumidity = biomeConditions.humidity.min - humidity;
	double deltaTemp = biomeConditions.temperature.min - temperature;
	
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

	Biome::Conditions biomeConditions = node->biome->getConditions();
	file << indent << "Biome: " << node->biome->getName().alloc_c_string() \
		<< ", Elevation: " << biomeConditions.elevation.min \
		<< ", Humidity: " << biomeConditions.humidity.min \
		<< ", Temperature: " << biomeConditions.temperature.min \
		<< ", Axis: " << node->axis \
		<< std::endl;

	printToFile(node->right, file, indent + "\t");
}
