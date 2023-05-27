#include "Chunk.hpp"

Chunk::Chunk(int x) : cx{ x }, tileMap{ nullptr }
{
	/*
	Ref<Resource> tileset = ResourceLoader::get_singleton()->load("res://resources/tiles/tileset.tres");
	tileMap->set_tileset(Object::cast_to<TileSet>(tileset.ptr()));
	Ref<Resource> script = ResourceLoader::get_singleton()->load("res://Worldgen/TileMap.gd");
	tileMap->set_script(script.ptr());
	*/
}

Chunk::~Chunk()
{
	if (tileMap != nullptr) {
		tileMap->queue_free();
		tileMap = nullptr;
	}
}

void Chunk::instance(Ref<PackedScene> tileMapScene)
{
	tileMap = Object::cast_to<TileMap>(tileMapScene->instance());
}

chunkdata_t Chunk::load(String saveName, TerrainGenerator* tg)
{
	Ref<File> file;
	file.instance();
	String filepath = String("res://resources/saves/") + hashName(saveName);
	Error err = file->open(filepath, File::READ);
	if (err == Error::OK && file->get_len()) {
		chunkdata_t cd;
		// This will load all existing tiles.
		while (file->get_position() != file->get_len()) {
			int64_t x = file->get_32();
			int64_t y = file->get_32();
			int8_t id = (int8_t)file->get_8(); // Max tile count = 256
			cd.push_back({ x, y, id });
		}

		file->close();
		return cd;
	}
	else { // If the file couldn't be found, the chunk needs to be generated
		return tg->generateChunk(cx, tileMap);
	}
}

void Chunk::save(String saveName)
{
	Ref<File> file;
	file.instance();
	String filepath = String("res://resources/saves/") + hashName(saveName);
	Error err = file->open(filepath, File::WRITE);

	if (err == Error::OK) {
		Array& cells = tileMap->get_used_cells();
		for (int i = 0; i < cells.size(); i++) {
			Vector2 pos = cells[i];
			int64_t id = tileMap->get_cellv(pos);
			file->store_32(int64_t(pos.x));
			file->store_32(int64_t(pos.y));
			file->store_8(id); // Max tile count = 256
		}
		file->close();
	}
	else {
		Godot::print("Failed to save chunk " + String::num_int64(cx));
	}
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

String Chunk::hashName(String saveName)
{
	return saveName + String("_") + String::num_int64(cx);
}
