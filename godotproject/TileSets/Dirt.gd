extends TileMap

var map_size = Vector2(640, 64) # size of the procedural world
var dirt_tile_id = 0 # ID of the dirt tile in your TileSet
var dirt_threshold = 0.5 # threshold for dirt placement

func _ready():
	# Generate the procedural world
	generate_world()

func generate_world():
	# Clear the TileMap
	clear()

	# Initialize the TileMap with dirt tiles using cellular automata
	for x in range(0, map_size.x):
		for y in range(0, map_size.y):
			if randf() < dirt_threshold:
				set_cellv(Vector2(x, y), dirt_tile_id)
