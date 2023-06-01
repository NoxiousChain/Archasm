#include "TerrainGenerator.hpp"

TerrainGenerator::~TerrainGenerator()
{
	// Delete everything in the blockIndexMap. blockMap keys point to same objects, so only need to delete once
	// This also saves a headache with godot dictionaries/arrays
	for (auto& item : blockIndexMap) {
		delete item.second;
	}
	blockMap.clear();
	blockIndexMap.clear();
}

void TerrainGenerator::_register_methods()
{
	register_method("_ready", &TerrainGenerator::_ready);
}

void TerrainGenerator::_init()
{
	for (int i = 0; i < 4; i++) {
		biomeNoise[i].instance();
	}
	caveNoise.instance();
	biomes = KDTree();

	rand01 = RRange<float>(0, 1);
}

void TerrainGenerator::_ready()
{
	// Initialize all noise functions
	// I can also try just using one noise function and incrementing y for the different conditions
	for (int i = 0; i < 4; i++) {
		biomeNoise[i]->set_seed(time(nullptr));
		// Higher values -> more detailed features but longer computation time
		biomeNoise[i]->set_octaves(5); // recommend 3-6
		// Higher values -> rougher & more prominent terrain
		biomeNoise[i]->set_persistence(0.4f); // .3-.6
		// Higher values -> less frequent biome changes
		biomeNoise[i]->set_period(70); // 20-100
		// Higher values -> more irregular features
		biomeNoise[i]->set_lacunarity(2.f); // ~2.0
	}

	caveNoise->set_seed(time(nullptr));
	// Lower values -> less detailed noise = larger, more connected caves
	caveNoise->set_octaves(3); // 2-4
	// Determines # of small vs large caves. Need to test to see actual changes
	caveNoise->set_persistence(.5f); // .4-.6
	// Lower values -> more frequent caves | Higher values -> fewer, larger caves
	caveNoise->set_period(35); // 20-50
	caveNoise->set_lacunarity(2.f); // ~2.0

	// Get block data
	//loadBlockDataFromJSON("res://resources/tiles/tiles.json");
	loadBiomeDataFromJSON("res://resources/tiles/biomes.json");
}

void TerrainGenerator::setParent(Node2D* cm)
{
	this->cm = cm;
}

void TerrainGenerator::generateChunk(int chunkX, TileMap* tileMap)
{
	Ref<OpenSimplexNoise> locBiomeNoise[4];
	Ref<OpenSimplexNoise> locCaveNoise;

	// Duplicate noise maps for thread safety
	noiseMtx.lock();
	for (int i = 0; i < 4; i++) {
		locBiomeNoise[i] = biomeNoise[i]->duplicate();
	}
	locCaveNoise = caveNoise->duplicate();
	noiseMtx.unlock();

	// Chunk data
	vector<tuple<float, float, float>> data = generateChunkData(chunkX);

	// Data points that will be needed later
	float avgE = 0.f, avgH = 0.f, avgT = 0.f; // condition averages, sampled from a few points along the chunk
	RRange<int> heightRange = { INT_MAX, -INT_MAX };

	// GET BIOME TYPE --------------------------------------------------------------------------//

	// Sample a few points along the chunk to get an approximate biome value
	const static int SAMPLE_COUNT = 5; // how many total samples taken
	const static int STARTING_X = (CHUNK_WIDTH % SAMPLE_COUNT) / 2;  // where along the chunk to get the first sample
	
	for (int i = STARTING_X; i < CHUNK_WIDTH; i += CHUNK_WIDTH / SAMPLE_COUNT) {
		avgE += std::get<0>(data[i]);
		avgH += std::get<1>(data[i]);
		avgT += std::get<2>(data[i]);
	}
	avgE /= (float)SAMPLE_COUNT;
	avgH /= (float)SAMPLE_COUNT;
	avgT /= (float)SAMPLE_COUNT;
	// Get biome from averages
	kdMtx.lock();
	Biome biome = biomes.nearestNeighbor(avgE, avgH, avgT);
	kdMtx.unlock();
	if (biome.isValid())
		Godot::print(biome.toString());

	// -----------------------------------------------------------------------------------------//


	// Iterate through every cell
	for (int x = 0; x < CHUNK_WIDTH; x++) {
		float elevation = std::get<0>(data[x]);
		float humidity = std::get<1>(data[x]);
		float temperature = std::get<2>(data[x]);
		int height = int(elevation * CHUNK_HEIGHT);

		heightRange.min = min(height, heightRange.min);
		heightRange.max = max(height, heightRange.max);

		for (int y = -height + 10; y < 250; y++) {
			tileMap->set_cell(x, y, 6); // fill with debug block if no valid block
		}
	}
	Godot::print(String("max height: ") + String::num_int64(heightRange.max));

	// CAVE GENERATION -------------------------------------------------------------------------//

	// Get parameters from ChunkManager
	const static float caveChanceToStartAlive = cm->get("CAVE_CHANCE_TO_START_ALIVE");
	const static float caveMaxHeightWeight = cm->get("CAVE_MAX_HEIGHT_WEIGHT");

	// Build cellular automata map
	// Map is a bitset - 0=empty 1=filled
	const int caveMaxHeight{ heightRange.wrand(caveMaxHeightWeight) };
	//std::vector<std::bitset<CHUNK_WIDTH>> caveMap(caveMaxHeight);

	// Generate the cavemap
	//for (int x = 0; x < CHUNK_WIDTH; x++) {
	//	for (int y = -caveMaxHeight; y < 250; y++) {
	//		//caveMap[x][y] = isSolid(x, y, caveMaxHeight);
	//		if (isSolid(x, y, caveMaxHeight)) {
	//			tileMap->set_cell(x, y, -1);
	//		}
	//	}
	//}

	// -----------------------------------------------------------------------------------------//

}

float TerrainGenerator::mapv(float v, float ol, float oh, float nl, float nh)
{
	return (v - ol) * (nh - nl) / (oh - ol) + nl;
}

float TerrainGenerator::nnoise(float in) const
{
	return (in + 1.f) / 2.f;
}

float TerrainGenerator::cosp(float a, float b, float mu)
{
	const float PI = 3.1415927f;
	float mu2 = (1.f - cos(mu * PI)) / 2.f;
	return a * (1.f - mu2) + b * mu2;
}

bool TerrainGenerator::isSolid(int x, int y, int maxHeight)
{
	const static float caveThreshold = cm->get("CAVE_THRESHOLD");
	const static float caveThresholdYMod = cm->get("CAVE_THRESHOLD_Y_MODIFIER");
	float threshold = caveThreshold + caveThresholdYMod * (float)y / (float)maxHeight;

	float sum = getCaveNoise(x, y) + getCaveNoise(x + 1, y) + \
		getCaveNoise(x - 1, y) + getCaveNoise(x, y + 1) + \
		getCaveNoise(x, y - 1);

	return sum > threshold;
}

float TerrainGenerator::getCaveNoise(int x, int y)
{
	return caveNoise->get_noise_2d((real_t)x, (real_t)y);
}

std::vector<std::tuple<float, float, float>> TerrainGenerator::generateChunkData(int chunkX) const
{
	std::vector<std::tuple<float, float, float>> data(CHUNK_WIDTH);
	const float lacunarity = 1.5f;
	const float persistence = .55f;

	for (int x = 0; x < CHUNK_WIDTH; x++) {
		int worldX = chunkX * CHUNK_WIDTH + x;
	
		// Elevation
		float elevation = nnoise(biomeNoise[3]->get_noise_1d((float)worldX * .05f));
	
		float elevationCubed = (float)pow(elevation, 3);
		float flatWeight = 1 - elevationCubed;
		float mountainWeight = elevationCubed;
		
		// Height
		float height = nnoise(biomeNoise[0]->get_noise_1d((float)worldX * lacunarity)) * persistence;
		float adjusted_height = (flatWeight + mountainWeight) * height;
		const float fudgeFactor = 1.2f; // helps balance out exponent
		const float expFactor = 2.65f;	// higher value -> make middle elevations flatter / lower value -> make middle elevations steeper
		adjusted_height = pow(adjusted_height * fudgeFactor, expFactor);
		
		// Humidity
		float humidity = nnoise(biomeNoise[1]->get_noise_1d((float)worldX * lacunarity)) * persistence;
		float adjusted_humidity = (1.f - elevation) * humidity; // Adjust humidity based on elevation
		
		// Temperature
		float temperature = nnoise(biomeNoise[2]->get_noise_1d((float)worldX * lacunarity)) * persistence;
		float adjusted_temperature = (1.f - elevation) * temperature; // Adjust temperature based on elevation
		
		data[x] = make_tuple(adjusted_height, adjusted_humidity, adjusted_temperature);
	}

	return data;
}

void TerrainGenerator::loadBlockDataFromJSON(const String& filepath)
{
	File file;
	Error err = file.open(filepath, File::READ);
	if (err != Error::OK) {
		Godot::print("Failed to open block data JSON file at " + filepath);
		return;
	}

	String fileContent = file.get_as_text();
	file.close();

	JSON* json = JSON::get_singleton();
	Ref<JSONParseResult> jpr = json->parse(fileContent);
	Dictionary data = jpr->get_result();
	err = jpr->get_error();
	if (err != Error::OK) {
		Godot::print("Failed to parse block data JSON file at " + filepath);
	}

	Array blocks = data["blocks"];
	for (int i = 0; i < blocks.size(); i++) {
		const Dictionary& block = blocks[i];
		const String& name = block["name"];
		size_t index = block["index"];
		int64_t hardness = block["hardness"];
		bool surfaceOnly = false;
		if (block.has("surface_only")) surfaceOnly = bool(block["surfaceOnly"]);

		Block* newBlock = new Block(name, index, hardness, surfaceOnly);
		blockMap[name] = newBlock;
		//blockMap.insert({ name, newBlock });
		blockIndexMap.insert({ index, newBlock });
	}
	Array ores = data["ores"];
	for (int i = 0; i < ores.size(); i++) {
		const Dictionary& block = ores[i];
		const String& name = block["name"];
		size_t index = block["index"];
		int64_t hardness = block["hardness"];
		RRange<size_t> clumpSize = RRange<size_t>(size_t(block["clump_size_min"]), size_t(block["clump_size_max"]));
		RRange<double> depth = RRange<double>(double(block["depth_min"]), double(block["depth_max"]));
		double frequency = block["frequency"];

		Block* newOre = new Ore(name, index, hardness, clumpSize, depth, frequency);
		blockMap[name] = newOre;
		blockIndexMap.insert({ index, newOre });
	}
}

void TerrainGenerator::loadBiomeDataFromJSON(const String& filepath)
{
	
	// Open the file and store contents
	Ref<File> file;
	file.instance();
	Error err = file->open(filepath, File::READ);
	if (err != Error::OK) {
		Godot::print("Failed to open biome data JSON file at " + filepath);
		return;
	}

	String fileContent = file->get_as_text();
	file->close();
	
	// Parse JSON data
	JSON* json = JSON::get_singleton();
	Ref<JSONParseResult> jpr = json->parse(fileContent);
	Dictionary data = jpr->get_result();
	err = jpr->get_error();
	if (err != Error::OK) {
		Godot::print("Failed to parse biome data JSON file at " + filepath);
		return;
	}
	
	Array& biomesArr = Array(data["biomes"]); // remove & if broken
	for (int i = 0; i < biomesArr.size(); i++) {
		// Get dictionary of values for block at index from json
		const Dictionary& jsonBlock = biomesArr[i];
		// Get data
		const String& name = jsonBlock["name"];
		const Dictionary& conditions = jsonBlock["conditions"];

		// Instantiate biome
		Biome biome(name, conditions); // KDTree will handle deletion

		// Add all blocks in biome to list
		const Array& blocks = jsonBlock["blocks"]; // remove & if broken
		for (int j = 0; j < blocks.size(); j++) {
			const Dictionary& block = blocks[j];
			const String& name = block["type"];
			double frequency = block["frequency"];
			biome.addBlock(name, frequency);
			// Will need to add flowers, trees etc.
			// Need to find a way to save entities, etc. (these may not be the responsibility of the chunkmanager, depends)
		}
		
		biomes.insert(biome);
	}
}
