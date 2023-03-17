extends TileMap
onready var noise = $noise.texture.noise

# Called when the node enters the scene tree for the first time.
func _ready():
	randomize()
	noise.seed = randi()%1000
	for x in 200:
		var ground = abs(noise.get_noise_2d(x,0)*60)
		for y in range(ground, 200):
			var title_id = noise.get_noise_2d(x,y)
			if noise.get_noise_2d(x,y) > -0.25:
				set_cell(x,y,title_id)
