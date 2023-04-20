extends TileMap

var max_x = 500
var max_y = 500
var mount_range_y = 20
var mount_range_x = 50
var space_between_mounts = 90

enum {
	treeTop = 3
	treeTrunk = 2
	treeLog = 1
}

var dirt_noise = OpenSimplexNoise.new()

func _ready():
	dirt_noise.seed = randi()%1000
	dirt_noise.period = mount_range_x
	dirt_noise.persistence = 0.3

	for x in range(max_x):
		for y in range(max_y):
			var noise_height = dirt_noise.get_noise_1d(x)
			if ceil(noise_height * mount_range_y) < y or y > space_between_mounts:
				set_cellv(Vector2(x, y), 0)
			if randi()%5 == 4 and get_cell(x,y-1) == -1:
					create_tree(x,y-2,randi()%4 + 4,true)

	update_bitmask_region(Vector2(0,0), Vector2(max_x, max_y))

func create_tree(x,y,length,new):
	if new:
		for i in length:
			if get_cell(x,y-i) != -1:
				break
				length = 0

	if length > 0:
		if new: set_cell(x,y, treeLog)
		elif length == 1: set_cell(x,y-1,treeTop)
		else: set_cell(x,y,treeTrunk)
		create_tree(x,y-1,length-1,false)


func _process(delta):
	if Input.is_action_pressed("mb_left"):
		var mouse_pos = get_global_mouse_position()
		var map_pos = world_to_map(mouse_pos)
		var tile_id = get_cellv(map_pos)
		if tile_id != -1:
			set_cellv(map_pos, -1)
