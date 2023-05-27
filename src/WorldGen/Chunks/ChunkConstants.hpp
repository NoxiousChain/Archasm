#pragma once
#include <vector>

// Chunk and cell size constants
const size_t CHUNK_WIDTH = 32;
const size_t CHUNK_HEIGHT = 256;
const size_t CELL_SIZE = 16;
const size_t HIDDEN_CHUNKS = 4;
// # of samples for linear/cosine interpolation
const size_t NUM_SAMPLES = 2;

struct ChunkData {
	int64_t x, y;
	int8_t id;
};

typedef vector<ChunkData> chunkdata_t;