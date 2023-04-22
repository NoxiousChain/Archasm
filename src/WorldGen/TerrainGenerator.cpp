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
		Array conditions = json_block["conditions"];

		// Instantiate block and add to KD tree
		BlockType block(name, index, conditions);
		blockTypes.insert(block);
	}
}
