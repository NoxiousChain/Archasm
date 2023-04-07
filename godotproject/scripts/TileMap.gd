extends TileMap

export(int) var max_x = 500
export(int) var max_y = 500

var noise : OpenSimplexNoise = OpenSimplexNoise.new()
var noise_gems : OpenSimplexNoise = OpenSimplexNoise.new()

var snap_size_x = 16
var snap_size_y = 16

var debug : bool = false

onready var selector = $selector

enum {
	flower1 = 35
	flower2 = 36
	treeLeaves = 37
	treeTrunk = 38
	treeBase = 39
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
	noise_gems.persistence = 0
	noise_gems.lacunarity = 0.1
	

func generate_level():
	noise.seed = randi()%1000
	for i in range(10):
		set_cell(63+i,0,0)
		set_cell(63+i,1,0)
	
	for x in max_x:
		var ground = abs(noise.get_noise_2d(x,0)*5)
		for y in range(ground, max_y):
			var title_id = generate_id(noise.get_noise_2d(x,y))
			if randi()%1 == 1 and get_cell(x,y-1) == -1:
					set_cell(x,y-1, title_id)
			if(title_id != -1):
				set_cell(x,y,title_id)
				if y < 30:
					if randi()%4 == 1 and get_cell(x,y-1) == -1:
						set_cell(x,y-1,flower1)
					if randi()%4 == 1 and get_cell(x,y-1) == -1:
						set_cell(x,y-1,flower2)
					if randi()%5 == 4 and get_cell(x,y-1) == -1:
						create_tree(x,y-4,randi()%4 + 4,true)


func generate_gems():
	for x in max_x:
		for y in max_y:
			if(get_cell(x,y) != -1): # there is mud here to place gem
				
				if(noise_gems.get_noise_2d(x,y) > 0.5):
					
					#Get percentage of the way down
					var percent : int = (float(y) / float(max_y)) * 100
					var gemID = 0 # mud
					
					#Further down the more rare the gem
					if(percent in range(3.5,20)):
						if(rand_range(0,1)) > 0.05: # 95%
							gemID = 5
						else:
							gemID = 10 #5% chance of a better gem
					elif(percent in range(21,40)):
						if(rand_range(0,1)) > 0.2: # 80%
							gemID = 10
						else:
							gemID = 15 #20% chance of a better gem
					elif(percent in range(41,60)):
						if(rand_range(0,1)) > 0.2: # 80%
							gemID = 15
						else:
							gemID = 20 #20% chance of a better gem
					elif(percent in range(61,80)):
						if(rand_range(0,1)) > 0.05: # 95%
							gemID = 20
						else:
							gemID = 25 #5% best gem
					elif(percent in range(81,90)):
						if(rand_range(0,1)) > 0.2: # 80%
							gemID = 25
						else:
							gemID = 30 #20% chance of the best gem
					elif(percent in range(90,100)):
						if(rand_range(0,1)) > 0.2: # 80%
							gemID = 30
						else:
							gemID = 34 #20% chance of the best gem
					

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

func _physics_process(_delta: float) -> void:
	if(Input.is_action_pressed("mb_left")):
		var title : Vector2 = world_to_map(selector.mouse_pos * 8)
		var title_id = get_cellv(title)
		var new_id = -1
		
		if(title_id != -1):
			if(title_id < 4):
				new_id = (title_id +1)
			else:
				new_id = -1
			set_cellv(title,new_id)
			
	if(Input.is_action_pressed("mb_right")):
		var title: Vector2 = world_to_map(selector.mouse_pos * 8)
		set_cellv(title,0)
		

func _input(event):
	if Input.is_action_pressed("paused"):
		get_tree().change_scene("res://scenes/GamePause.tscn")
