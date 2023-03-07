extends TileMap

export(int) var max_x = 500
export(int) var max_y = 500

onready var selector : Sprite = $selector
var noise : OpenSimplexNoise = OpenSimplexNoise.new()

var snap_size_x = 16
var snap_size_y = 16

func _ready() -> void:
	randomize()
	noise.seed = randi()%1
	generate_level()
	
func generate_level():
	for x in max_x:
		var ground = abs(noise.get_noise_2d(x,0)*10)
		for y in range(ground, max_y):
			var title_id = generate_id(noise.get_noise_2d(x,y))
			if(title_id != -1):
				set_cell(x,y,title_id)
	
	for i in range(10):
		set_cell(63+i,0,0)
		set_cell(63+i,1,0)
			
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
		

func _input(event):
	if Input.is_action_pressed("ui_cancel"):
		get_tree().change_scene("res://Menu.tscn")
