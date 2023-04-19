extends Node2D

var noise = OpenSimplexNoise.new()

func _ready():
	for x in range(30):
		for y in range(20):
			$chunk.set_cellv(Vector2(x, y), 0)
	$chunk.update_bitmask_region(Vector2(0,0), Vector2(30,20))
