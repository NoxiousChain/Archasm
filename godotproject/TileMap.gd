extends TileMap

export(int) var max_x = 500
export(int) var max_y = 500

onready var selector : Sprite = $selector
var noise : OpenSimplexNoise = OpenSimplexNoise.new()

var snap_size_x = 16
var snap_size_y = 16

enum {
	flower = 5
	flower2 = 6
	tree = 7
	treebase = 8
	grass = 9
}

func _ready() -> void:
	randomize()
	noise.seed = randi()%1000
	generate_level()
	
func generate_level():
	for x in max_x:
		var ground = abs(noise.get_noise_2d(x,0)*5)
		
		for y in range(ground, max_y):
			var title_id = generate_id(noise.get_noise_2d(x,y))
			if randi()%1 == 1 and get_cell(x,y-1) == -1:
					set_cell(x,y-1,grass)
			if(title_id != -1):
				set_cell(x,y,title_id)
				if randi()%4 == 1 and get_cell(x,y-1) == -1:
					set_cell(x,y-1,flower)
				if randi()%4 == 1 and get_cell(x,y-1) == -1:
					set_cell(x,y-1,flower2)
				if randi()%5 == 4 and get_cell(x,y-1) == -1:
					create_tree(x,y-1,randi()%4 + 4,true)
					
func create_tree(x,y,length,new):
	if new:
		for i in length:
			if get_cell(x,y-i) != -1:
				break
				length = 0
	if length > 0:
		if new: set_cell(x,y,treebase)
		elif length == 1: set_cell(x,y-1,tree)
		else: set_cell(x,y,treebase)
		create_tree(x,y-1,length-1,false)

	for i in range(10):
		set_cell(63+i,0,0)
		set_cell(63+i,1,0)
			
func generate_id(noise_level : float):
		if(noise_level <= -0.1):
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
	if Input.is_action_pressed("paused"):
		get_tree().change_scene("res://GamePause.tscn")
