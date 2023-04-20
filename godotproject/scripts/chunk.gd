extends TileMap

enum {
	AIR = -1,
	PLAINE = 0,
	MONTAGNE = 1,
	SOUTERRAIN = 2,
	SOUTERRAIN_CORROMPU = 3,
}

signal remove

onready var main = get_node("../..")
onready var chunk_length = 496

var object_inst = {
	Tex.TRUNK_TOP: preload("res://scenes/Objects/Tree_top.tscn"),
}

