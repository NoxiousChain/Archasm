extends Node

onready var cm = $ChunkManager

func _ready():
	pass

func _exit_tree():
	cm.save_all_chunks()
	cm.delete_all_chunks()
