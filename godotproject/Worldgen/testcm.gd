extends Node

onready var cm = $ChunkManager

# these need to match values in ChunkConstants.hpp
const CELL_SIZE: int = 16
const CHUNK_WIDTH: int = 32

func _ready():
	pass

# automatically saves all chunks when the game exits
func _exit_tree():
	cm.save_all_chunks()
	cm.delete_all_chunks()

func world_to_chunk_x(worldX: int):
# warning-ignore:integer_division
	return worldX / (CELL_SIZE * CHUNK_WIDTH)

func chunk_to_world_x(chunkX: int):
	return chunkX * CELL_SIZE * CHUNK_WIDTH
