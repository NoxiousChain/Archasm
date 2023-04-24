#include "TerrainGenerator.hpp"

void TerrainGenerator::_register_methods()
{
	register_method("_ready", &TerrainGenerator::_ready);
}

void TerrainGenerator::_init()
{

}

void TerrainGenerator::_ready()
{
	rng = RandomNumberGenerator::_new();
	rng->set_seed(time(nullptr));

	// Initialize all noise functions

	// I can also try just using one noise function and incrementing y for the different conditions
	for (int i = 0; i < 3; i++) {
		biomeNoise[i] = OpenSimplexNoise::_new();
		biomeNoise[i]->set_seed(rng->randi());
		// Higher values -> more detailed features but longer computation time
		biomeNoise[i]->set_octaves(5); // recommend 3-6
		// Higher values -> rougher & more prominent terrain
		biomeNoise[i]->set_persistence(0.5); // .3-.6
		// Higher values -> less frequent biome changes
		biomeNoise[i]->set_period(50); // 20-100
		// Higher values -> more irregular features
		biomeNoise[i]->set_lacunarity(2.0); // ~2.0
	}

	heightNoise = OpenSimplexNoise::_new();
	heightNoise->set_seed(rng->randi());
	heightNoise->set_octaves(6); // 4-8
	// Determines smoothness vs roughness
	heightNoise->set_persistence(.55); // .4-.7
	// Lower values -> more frequent height changes
	heightNoise->set_period(100); // 50-150
	heightNoise->set_lacunarity(2.0); // ~2.0

	caveNoise = OpenSimplexNoise::_new();
	caveNoise->set_seed(rng->randi());
	// Lower values -> less detailed noise = larger, more connected caves
	caveNoise->set_octaves(3); // 2-4
	// Determines # of small vs large caves. Need to test to see actual changes
	caveNoise->set_persistence(.5); // .4-.6
	// Lower values -> more frequent caves | Higher values -> fewer, larger caves
	caveNoise->set_period(35); // 20-50
	caveNoise->set_lacunarity(2.0); // ~2.0

	// Get block data
	loadBlockDataFromJSON("res://resources/tiles/tiles.json");
}

Ref<TileMap> TerrainGenerator::generateChunk(int chunkX)
{
	Ref<TileMap> tilemap = TileMap::_new();

	// These are values for the chunk as a whole. 
	// They will influence the other generation points
	float elevation = biomeNoise[0]->get_noise_1d(chunkX);
	float humidity = biomeNoise[1]->get_noise_1d(chunkX);
	float temperature = biomeNoise[2]->get_noise_1d(chunkX);

	// Iterate through every cell
	for (int x = 0; x < CHUNK_WIDTH; x++) {
		float heightGen = heightNoise->get_noise_1d(x);

		for (int y = 0; y < CHUNK_HEIGHT; y++) {
			

		}
	}
}

int TerrainGenerator::getBlockTypeFromNoise(float elevation, float humidity, float temperature)
{
	BlockType blockData = blockTypes.nearestNeighbor(elevation, humidity, temperature);
	int blockType = static_cast<int>(blockData.getIndex());
	return blockType;
}

float TerrainGenerator::mapv(float v, float ol, float oh, float nl, float nh)
{
	return nl + (v * ((nh - nl) / (oh - ol)));
}

float TerrainGenerator::cosp(float a, float b, float mu)
{
	const float PI = 3.1415927f;
	float mu2 = (1 - cos(mu * PI)) / 2;
	return a * (1 - mu2) + b * mu2;
}

std::vector<float> TerrainGenerator::getElevationNoise(int chunkX)
{
	std::vector<float> generated;
	int tileX = chunkX * CHUNK_WIDTH;
	for (int i = tileX; i < tileX + CHUNK_WIDTH; i++) {
		generated.push_back(biomeNoise[0]->get_noise_1d(i));
	}
	return std::move(generated);
}

std::vector<float> TerrainGenerator::getHumidityNoise(int chunkX)
{
	std::vector<float> generated;
	int tileX = chunkX * CHUNK_WIDTH;
	for (int i = tileX; i < tileX + CHUNK_WIDTH; i++) {
		generated.push_back(biomeNoise[1]->get_noise_1d(i));
	}
	return std::move(generated);
}

std::vector<float> TerrainGenerator::getTemperatureNoise(int chunkX)
{
	std::vector<float> generated;
	int tileX = chunkX * CHUNK_WIDTH;
	for (int i = tileX; i < tileX + CHUNK_WIDTH; i++) {
		generated.push_back(biomeNoise[2]->get_noise_1d(i));
	}
	return std::move(generated);
}

std::vector<float> TerrainGenerator::getHeightNoise(int chunkX)
{
	std::vector<float> generated;
	int tileX = chunkX * CHUNK_WIDTH;
	for (int i = tileX; i < tileX + CHUNK_WIDTH; i++) {
		generated.push_back(heightNoise->get_noise_1d(i));
	}
	return std::move(generated);
}

std::vector<float> TerrainGenerator::terrainSuperposCosp(int chunkX, int iterations)
{
	std::vector<std::vector<float>> terrains;
	float weightSum = 0;

	// Get all points in chunk from height & elevation noise maps
	std::vector<float> heightNoise = getHeightNoise(chunkX);
	std::vector<float> elevationNoise = getElevationNoise(chunkX);

	size_t generatedSize = generated.size();
	std::vector<float> result(generatedSize, 0.f);

	for (int z = iterations; z > 0; z--) {
		float weight = 1 / std::pow(2, z - 1);
		int sample = 1 << (iterations - z);

		weightSum += weight;

		for (int i = 0, j = 0; i < generatedSize; i += sample, j++) {
			float a = heightNoise[i] * (1 + elevationNoise[i]); // Modify the height using elevation. Just linear for now, might want to make it more interesting later
			float b = heightNoise[(i + sample) % generatedSize] * (1 + elevation[(i + sample) % generatedSize];

			result[i] += weight * a;

			for (int k = 1; k < sample; k++) {
				float mu = static_cast<float>(k) / sample;
				float v = cosp(a, b, mu);

				result[(i + k) % generatedSize] += weight * v;
			}
		}
	}

	for (float& value : result) {
		value /= weightSum;
	}

	return std::move(result);
}

void TerrainGenerator::loadBlockDataFromJSON(const String& filepath)
{
	// Open the file and store contents
	File file;
	Error err = file.open(filepath, File::READ);
	if (err != Error::OK) {
		Godot::print_error("Failed to open tile data JSON file");
		return;
	}

	String fileContent = file.get_as_text();
	file.close();

	// Parse JSON data
	Variant data;
	err = JSON::parse(fileContent, data);
	if (err != Error::OK) {
		Godot::print_error("Failed to parse tile data JSON file");
		return;
	}

	// Load block data into dictionary
	Array& blocks = data["blocks"];
	for (int i = 0; i < blocks.size(); i++) {
		// Get dictionary of values for block at index from json
		const Dictionary& json_block = blocks[i];
		// Get data
		String name = json_block["name"];
		size_t index = json_block["index"];
		Array& conditions = json_block["conditions"];

		// Instantiate block and add to KD tree
		BlockType block(name, index, conditions);
		blockTypes.insert(block);
	}
}
