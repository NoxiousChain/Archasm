#include "Chunk.hpp"

Chunk::Chunk(int x) : cx{ x }
{
	tileMap->set_cell_size(Vector2(CELL_SIZE, CELL_SIZE));
}

void Chunk::load(const String& saveName, TerrainGenerator* tg)
{
	Ref<File> file;
	Error err = file->open(hashName(saveName), File::READ);

	if (err == Error::OK) {
		tileMap->clear();

		// This will load all existing tiles.
		while (file->get_position() < file->get_len()) {
			int64_t x = file->get_32();
			int64_t y = file->get_32();
			int64_t id = file->get_32();
			tileMap->set_cell(x, y, id);
		}

		file->close();
	}
	else { // If the file couldn't be found, the chunk needs to be generated
		tileMap = tg->generateChunk(cx);
	}
}

void Chunk::save(const String& saveName)
{
	Ref<File> file = File::_new();
	String filepath = hashName(saveName);
	Error err = file->open(filepath, File::WRITE);

	if (err == Error::OK) {
		for (int64_t x = 0; x < CHUNK_WIDTH; x++) {
			for (int64_t y = 0; y < CHUNK_HEIGHT; y++) {
				int64_t id = tileMap->get_cell(x, y);

				// Only save existing tiles
				if (id != -1) {
					file->store_32(x);
					file->store_32(y);
					file->store_32(id);
				}
			}
		}
	}
	else {
		Godot::print("Failed to save chunk " + String::num_int64(cx));
	}

	file->close();
}

int Chunk::getX() const 
{
	return cx;
}

TileMap* Chunk::getTileMap() const
{
	return tileMap;
}

String Chunk::hashName(const String& saveName)
{
	String combined = saveName + String::num_int64(cx);
	int hash = combined.hash();

	return "chunk_" + String::num_int64(hash);
}
