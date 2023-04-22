#include "TerrainGenerator.hpp"

void TerrainGenerator::_register_methods()
{

}

void TerrainGenerator::_init()
{

}

void TerrainGenerator::_ready()
{

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
	const Array& blocks = data["blocks"];
	pointVec points;
	point_t pt;
	for (int i = 0; i < blocks.size(); i++) {
		pt = {};
		blockTypes.
	}
}
