extends TileMap

export(int) var max_x = 200
export(int) var max_y = 200

onready var selector : Sprite = $selector

var noise : OpenSimplexNoise = OpenSimplexNoise.new()

var snap_size_x = 16
var snap_size_y = 16

func _ready() -> void:
	randomize()
	
	noise.seed = randi()
	noise.octaves = 2
	noise.period = 30
	noise.persistence = 1
	noise.lacunarity = 4
	
	generate_level()
	
func generate_level():
	for x in max_x:
		for y in max_y:
			var title_id = generate_id(noise.get_noise_2d(x,y))
			if(title_id != -1):
				set_cell(x,y,title_id)
			
func generate_id(noise_level : float):
		if(noise_level <= -0.05):
			return -1
		else:
			return 0

func _physics_process(_delta: float) -> void:
	if(Input.is_action_pressed("mb_left")):
		var title : Vector2 = world_to_map(selector.mouse_pos * 8)
		var title_id = get_cellv(title)
		
		var new_id = -1
		
		if(title_id != -1):
			if(title_id < 5):
				new_id = (title_id +1)
			else:
				new_id = -1
			set_cellv(title,new_id)
	if(Input.is_action_pressed("mb_right")):
		var title: Vector2 = world_to_map(selector.mouse_pos * 8)
		set_cellv(title,0)
