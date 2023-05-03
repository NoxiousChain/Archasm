#include "TerrainGenerator.hpp"

TerrainGenerator::~TerrainGenerator()
{
}

void TerrainGenerator::_register_methods()
{
	register_method("_ready", &TerrainGenerator::_ready);
}

void TerrainGenerator::_init()
{
	for (int i = 0; i < 3; i++) {
		biomeNoise[i].instance();
	}
	heightNoise.instance();
	caveNoise.instance();
}

void TerrainGenerator::_ready()
{
	// Initialize all noise functions

	// I can also try just using one noise function and incrementing y for the different conditions
	for (int i = 0; i < 3; i++) {
		biomeNoise[i]->set_seed(time(nullptr));
		// Higher values -> more detailed features but longer computation time
		biomeNoise[i]->set_octaves(5); // recommend 3-6
		// Higher values -> rougher & more prominent terrain
		biomeNoise[i]->set_persistence(0.6f); // .3-.6
		// Higher values -> less frequent biome changes
		biomeNoise[i]->set_period(70); // 20-100
		// Higher values -> more irregular features
		biomeNoise[i]->set_lacunarity(2.f); // ~2.0
	}

	heightNoise->set_seed(time(nullptr));
	heightNoise->set_octaves(6); // 4-8
	// Determines smoothness vs roughness
	heightNoise->set_persistence(.45f); // .4-.7
	// Lower values -> more frequent height changes
	heightNoise->set_period(120); // 50-150
	heightNoise->set_lacunarity(2.f); // ~2.0

	caveNoise->set_seed(time(nullptr));
	// Lower values -> less detailed noise = larger, more connected caves
	caveNoise->set_octaves(3); // 2-4
	// Determines # of small vs large caves. Need to test to see actual changes
	caveNoise->set_persistence(.5f); // .4-.6
	// Lower values -> more frequent caves | Higher values -> fewer, larger caves
	caveNoise->set_period(35); // 20-50
	caveNoise->set_lacunarity(2.f); // ~2.0

	// Get block data
	blockTypes = KDTree();
	loadBlockDataFromJSON("res://resources/tiles/tiles.json");
}

void TerrainGenerator::generateChunk(int chunkX, TileMap* tileMap)
{
	// These are values for the chunk as a whole. 
	// They will influence the other generation points
	//float elevation = getElevationNoise(chunkX);
	//float humidity = getHumidityNoise(chunkX);
	//float temperature = getTemperatureNoise(chunkX);

	//std::vector<float> terrain = terrainSuperposCosp(chunkX);
	std::vector<float> terrain = generateChunkHeights(chunkX);

	// Iterate through every cell
	for (int x = 0; x < terrain.size(); x++) {
		int height = int(terrain[x] * CHUNK_HEIGHT) / 2;
	
		for (int y = -height + 10; y < 250; y++) {
			tileMap->set_cell(x, y, 0); // fill with dirt
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
	return (v - ol) * (nh - nl) / (oh - ol) + nl;
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
		float noise = biomeNoise[0]->get_noise_1d((float)i);
		//float normalized = (noise + 1.f) * .5f;
		//generated.push_back(normalized);
		generated.push_back(mapv(noise, -1, 1, 0, 1));
	}
	return std::move(generated);
}

std::vector<float> TerrainGenerator::getHumidityNoise(int chunkX)
{
	std::vector<float> generated;
	int tileX = chunkX * CHUNK_WIDTH;
	for (int i = tileX; i < tileX + CHUNK_WIDTH; i++) {
		float noise = biomeNoise[1]->get_noise_1d((float)i);
		//float normalized = (noise + 1.f) * .5f;
		//generated.push_back(normalized);
		generated.push_back(mapv(noise, -1, 1, 0, 1));
	}
	return std::move(generated);
}

std::vector<float> TerrainGenerator::getTemperatureNoise(int chunkX)
{
	std::vector<float> generated;
	int tileX = chunkX * CHUNK_WIDTH;
	for (int i = tileX; i < tileX + CHUNK_WIDTH; i++) {
		float noise = biomeNoise[2]->get_noise_1d((float)i);
		//float normalized = (noise + 1.f) * .5f;
		//generated.push_back(normalized);
		generated.push_back(mapv(noise, -1, 1, 0, 1));
	}
	return std::move(generated);
}

std::vector<float> TerrainGenerator::smoothNoiseMap(const std::vector<float>& noiseMap, int windowSize)
{
	size_t size = noiseMap.size();
	std::vector<float> smoothed(size);

	for (int i = 0; i < size; i++) {
		float sum = 0;
		int count = 0;
		for (int j = i - windowSize / 2; j <= i + windowSize / 2; j++) {
			if (j >= 0 && j < size) {
				sum += noiseMap[j];
				count++;
			}
		}
		smoothed[i] = sum / count;
	}

	return smoothed;
}

std::vector<float> TerrainGenerator::getHeightNoise(int chunkX)
{
	std::vector<float> generated;
	int tileX = chunkX * CHUNK_WIDTH;
	for (int i = tileX; i < tileX + CHUNK_WIDTH; i++) {
		float noise = heightNoise->get_noise_1d((float)i);
		//float normalized = (noise + 1.f) * .5f;
		//generated.push_back(normalized);
		generated.push_back(mapv(noise, -1, 1, 0, 1));
	}
	return std::move(generated);
}

std::vector<float> TerrainGenerator::terrainSuperposCosp(int chunkX, int iterations)
{
	//std::vector<std::vector<float>> terrains;
	float weightSum = 0;

	// Get all points in chunk from height & elevation noise maps
	std::vector<float> heightNoise = getHeightNoise(chunkX);
	std::vector<float> elevationNoise = getElevationNoise(chunkX);

	size_t generatedSize = heightNoise.size();
	std::vector<float> result(generatedSize, 0.f);

	for (int z = iterations; z > 0; z--) {
		float weight = 1.f / static_cast<float>(std::pow(2, z - 1));
		int sample = 1 << (iterations - z);

		weightSum += weight;

		for (int i = 0, j = 0; i < generatedSize; i += sample, j++) {
			float a = heightNoise[i] * (1 + elevationNoise[i]); // Modify the height using elevation. Just linear for now, might want to make it more interesting later
			float b = heightNoise[(i + sample) % generatedSize] * (1 + elevationNoise[(i + sample) % generatedSize]);

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

std::vector<float> TerrainGenerator::generateChunkHeights(int chunkX)
{
	std::vector<float> heights(CHUNK_WIDTH);
	const float lacunarity = 2.f;
	const float persistence = .55f;

	for (int x = 0; x < CHUNK_WIDTH; x++) {
		int worldX = chunkX * CHUNK_WIDTH + x;
		float elevation = (biomeNoise[0]->get_noise_1d((float)worldX * .05f) + 1) / 2; // normalized
		
		float elevationCubed = (float)pow(elevation, 3);
		float flatWeight = 1 - elevationCubed;
		float mountainWeight = elevationCubed;
		
		float height = heightNoise->get_noise_1d((float)worldX * lacunarity) * persistence;
		float adjusted = (flatWeight + mountainWeight) * height * 4;

		heights[x] = mapv(adjusted, -1, 1, 0, 1) / 4;
	}

	return heights;
}

void TerrainGenerator::loadBlockDataFromJSON(const String& filepath)
{
	// Open the file and store contents
	File file;
	Error err = file.open(filepath, File::READ);
	if (err != Error::OK) {
		Godot::print("Failed to open tile data JSON file");
		return;
	}

	String fileContent = file.get_as_text();
	file.close();

	// Parse JSON data
	JSON* json = JSON::get_singleton();
	Ref<JSONParseResult> jpr;
	jpr = json->parse(fileContent);
	Dictionary data = jpr->get_result();
	err = jpr->get_error();
	if (err != Error::OK) {
		Godot::print("Failed to parse tile data JSON file");
		return;
	}
	Array blocks = Array(data["blocks"]);
	for (int i = 0; i < blocks.size(); i++) {
		// Get dictionary of values for block at index from json
		const Dictionary& jsonBlock = blocks[i];
		// Get data
		String name = jsonBlock["name"];
		int64_t index = jsonBlock["index"];
		Dictionary conditions = jsonBlock["conditions"];

		// Instantiate block and add to KD tree
		BlockType block(name, conditions, (size_t)index);

		// THIS FUNCTION CALL CRASHES GODOT FOR SOME FUCKING REASON I CANT FIGURE OUT
		//blockTypes.insert(block);
	}
}
