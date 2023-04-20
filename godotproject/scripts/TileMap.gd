extends TileMap

export(int) var max_x = 500
export(int) var max_y = 100

var noise : OpenSimplexNoise = OpenSimplexNoise.new()
var noise_gems : OpenSimplexNoise = OpenSimplexNoise.new()

var snap_size_x = 16
var snap_size_y = 16

var debug : bool = false

onready var selector = $"/root/Selector"

var interact_enabled = true

enum {
	flower1 = 11
	flower2 = 12
	treeLeaves = 9
	treeTrunk = 10
	treeBase = 8
	dirt = 18
}


func _ready() -> void:
	randomize()
	
	init_noise_gems()
	generate_level()
	generate_gems()


func init_noise_gems():
	noise_gems.seed = randi()
	noise_gems.octaves = 9
	noise_gems.period = 6.6
	noise_gems.persistence = 0.15
	noise_gems.lacunarity = 0.1
	

func generate_level():
	noise.seed = randi()%1000
	for i in range(10):
		set_cell(63+i,0,0)
		set_cell(63+i,1,0)
	
	for x in max_x:
		var ground = abs(noise.get_noise_2d(x,0)*5)
		for y in range(ground, max_y):
#			var dirt = generate_id(noise.get_noise_2d(x,y))
			if randi()%1 == 1 and get_cell(x,y-1) == -1:
					set_cell(x,y-1, dirt)
			if(dirt != -1):
				set_cell(x,y,dirt)
			if y < 30:
				if randi()%4 == 1 and get_cell(x,y-1) == -1:
					set_cell(x,y-1,flower1)
				if randi()%4 == 1 and get_cell(x,y-1) == -1:
					set_cell(x,y-1,flower2)
				if randi()%5 == 4 and get_cell(x,y-1) == -1:
					create_tree(x,y-4,randi()%4 + 4,true)
	
	update_bitmask_region(Vector2(0,0), Vector2(max_x, max_y))

func generate_gems():
	for x in max_x:
		for y in max_y:
			if(get_cell(x,y) != -1): # there is mud here to place gem
				
				if(noise_gems.get_noise_2d(x,y) > 0.5):
					
					#Get percentage of the way down
					var percent : int = int((float(y) / float(max_y)) * 100)
					var gemID = 0 # mud
					
					#Further down the more rare the gem
					if(percent in range(3.5,20)):
						if(rand_range(0,1)) > 0.05: # 95%
							gemID = 2
						else:
							gemID = 3 #5% chance of a better gem
					elif(percent in range(21,40)):
						if(rand_range(0,1)) > 0.2: # 80%
							gemID = 3
						else:
							gemID = 4 #20% chance of a better gem
					elif(percent in range(41,60)):
						if(rand_range(0,1)) > 0.2: # 80%
							gemID = 4
						else:
							gemID = 5 #20% chance of a better gem
					elif(percent in range(61,80)):
						if(rand_range(0,1)) > 0.05: # 95%
							gemID = 5
						else:
							gemID = 6 #5% best gem
					elif(percent in range(81,90)):
						if(rand_range(0,1)) > 0.2: # 80%
							gemID = 6
						else:
							gemID = 6 #20% chance of the best gem
					elif(percent in range(90,100)):
						if(rand_range(0,1)) > 0.2: # 80%
							gemID = 7
						else:
							gemID = 7 #20% chance of the best gem
					

					set_cell(x,y,gemID)

func create_tree(x,y,length,new):
	if new:
		for i in length:
			if get_cell(x,y-i) != -1:
				break
				length = 0

	if length > 0:
		if new: set_cell(x,y,treeBase)
		elif length == 1: set_cell(x,y-1,treeLeaves)
		else: set_cell(x,y,treeTrunk)
		create_tree(x,y-1,length-1,false)

func generate_id(noise_level : float):
		if(noise_level <= -0.1):
			return -1
		else:
			return 0

func _process(delta):
	var mouse_pos = get_global_mouse_position()
	var map_pos = world_to_map(mouse_pos)
	var tile_id = get_cellv(map_pos)
	if interact_enabled:
		if Input.is_action_pressed("mb_left"):
			if tile_id != -1:
				set_cellv(map_pos, -1)
		if(Input.is_action_pressed("mb_right")):
			set_cellv(map_pos, 0)

func _input(_event):
	if Input.is_action_pressed("paused"):
		var _err = get_tree().change_scene("res://scenes/GamePause.tscn")

func _toggle_interact():
	interact_enabled = !interact_enabled
