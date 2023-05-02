extends TileMap

var interact_enabled = true

# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.


func _process(_delta):
	var mouse_pos = get_global_mouse_position()
	var map_pos = world_to_map(mouse_pos)
	if interact_enabled:
		if Input.is_action_pressed("mb_left"):
			set_cellv(map_pos, -1)
		if(Input.is_action_pressed("mb_right")):
			set_cellv(map_pos, 0)

func _toggle_interact():
	interact_enabled = !interact_enabled
