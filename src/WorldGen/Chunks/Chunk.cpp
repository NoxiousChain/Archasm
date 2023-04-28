#include "Chunk.hpp"

Chunk::Chunk(int x) : cx{ x }
{
	Godot::print("Chunk constructed");
	tileMap = TileMap::_new();
	tileMap->set_cell_size(Vector2(CELL_SIZE, CELL_SIZE));
	tileMap->set_position(Vector2(chunkToWorldX(cx), 0));
	Ref<Resource> loaded = ResourceLoader::get_singleton()->load("res://resources/tiles/tileset.tres");
	tileMap->set_tileset(loaded);
}

Chunk::~Chunk()
{
	if (tileMap != nullptr) {
		tileMap->queue_free();
		tileMap = nullptr;
	}
}

void Chunk::load(const String& saveName, TerrainGenerator* tg)
{
	Ref<File> file = File::_new();
	Error err = file->open(hashName(saveName), File::READ);

	if (err == Error::OK) {
		Godot::print("chunk loading from file...");
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
		Godot::print("tg generating chunk...");
		tg->generateChunk(cx, tileMap);
	}
}

void Chunk::save(const String& saveName)
{
	Ref<File> file = File::_new();
	String filepath = String("res://resources/saves/") + hashName(saveName);
	Error err = file->open(filepath, File::WRITE);

	if (err == Error::OK) {
		Godot::print("saving chunk...");
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
		file->close();
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

// chunkX starts at 0
int Chunk::worldToChunkX(int worldX)
{
	return worldX / (CELL_SIZE * CHUNK_WIDTH);
}

int Chunk::chunkToWorldX(int chunkX)
{
	return chunkX * CELL_SIZE * CHUNK_WIDTH;
}

String Chunk::hashName(const String& saveName)
{
	String combined = saveName + String::num_int64(cx);
	int hash = combined.hash();

	return "chunk_" + String::num_int64(hash);
}
