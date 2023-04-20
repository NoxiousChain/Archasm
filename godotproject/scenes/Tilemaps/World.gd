extends Node2D

var max_x = 500
var max_y = 500
var mount_range_y = 20
var mount_range_x = 50
var space_between_mounts = 90

var dirt_noise = OpenSimplexNoise.new()

var breaking = false

func _ready():
	dirt_noise.seed = randi()
	dirt_noise.period = mount_range_x
	dirt_noise.persistence = 0.3
	
	for x in range(max_x):
		for y in range(max_y):
			var noise_height = dirt_noise.get_noise_1d(x)
			if ceil(noise_height * mount_range_y) < y or y > space_between_mounts:
				$chunk.set_cellv(Vector2(x, y), 0)
	$chunk.update_bitmask_region(Vector2(0,0), Vector2(max_x, max_y))

func _process(delta):
	if Input.is_action_pressed("mb_left"):
		var mouse_pos = get_global_mouse_position()
		var map_pos = $chunk.world_to_map(mouse_pos)
		var tile_id = $chunk.get_cellv(map_pos)
		if tile_id != -1:
			$chunk.set_cellv(map_pos, -1)
