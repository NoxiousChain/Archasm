extends Node

onready var cm = $ChunkManager

func _ready():
	pass

# automatically saves all chunks when the game exits
func _exit_tree():
	cm.save_all_chunks()
	cm.delete_all_chunks()
