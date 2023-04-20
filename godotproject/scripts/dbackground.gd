extends TileMap

export(int) var max_x = 500
export(int) var max_y = 500

var noise : OpenSimplexNoise = OpenSimplexNoise.new()

var snap_size_x = 16
var snap_size_y = 16

func _ready() -> void:
	randomize()
	
	noise.seed = randi()
	noise.octaves = 0
	noise.period = 0
	noise.persistence = 0
	noise.lacunarity = 0
	
	generate_level()
	
func generate_level():
	for x in max_x:
		for y in max_y:
			var dirt = generate_id(noise.get_noise_2d(x,y))
			if(dirt != -1):
				set_cell(x,y,dirt)
	
			
func generate_id(noise_level : float):
		if(noise_level <= -0.05):
			return -1
		else:
			return 0
