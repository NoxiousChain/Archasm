extends TileMap

export(int) var max_x = 200
export(int) var max_y = 200
var noise : OpenSimplexNoise = OpenSimplexNoise.new()

var snap_size_x = 16
var snap_size_y = 16

func _ready() -> void:
	randomize()
	
	
	generate_level()
	
func generate_level():
	for x in max_x:
		for y in max_y:
			var title_id = generate_id(noise.get_noise_2d(x,y))
			if(title_id != -1):
				set_cell(x,y,title_id)
	for i in range(max_x):
		set_cell(i,0,0)
		set_cell(i,1,0)

func generate_id(noise_level : float):
		if(noise_level <= -0.05):
			return -1
		else:
			return 0
