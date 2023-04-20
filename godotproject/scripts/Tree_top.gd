extends Sprite


onready var chunk = get_node("../..")
onready var main = get_node("../../../..")

export var map_pos : Vector2

var typo = "tronc_tete"
var data

func _ready():
	pass
	
func setup(coord, _slot):
	config(coord)
		
func config(coord):
	
	set_owner(chunk)
	chunk.set_cell(coord.x, coord.y, Tex.TRUNK_TOP)
	
	position = coord * 31 + Vector2(16,16)
	map_pos = chunk.world_to_map(position - Vector2(16,16))

func remake():
	pass

func _process(_delta):
	if chunk.get_cell(map_pos.x, map_pos.y) != Tex.TRUNK_TOP:
		delete()

func delete():
	queue_free()
