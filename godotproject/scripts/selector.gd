extends Node2D

export(int) var snap_size_x = 16
export(int) var snap_size_y = 16

var mouse_pos : Vector2 = Vector2.ZERO


func _physics_process(_delta: float) -> void:
	update_position_snapped()

func update_position_snapped():
	mouse_pos = Vector2(int(get_global_mouse_position().x / snap_size_x),
						int(get_global_mouse_position().y / snap_size_y))
						
	global_position = mouse_pos * snap_size_x

func _input(event):
	# Check if the mouse button was pressed
	if event is InputEventMouseButton and event.button_index == BUTTON_LEFT and event.pressed:
		# Get the mouse position
		var mouse_pos = get_global_mouse_position()

		var space_state = get_world_2d().direct_space_state
		var result = space_state.intersect_ray(get_global_transform().origin, mouse_pos, [], 1)

		if result:
			var collided_object = result["collider"]
			if collided_object.is_in_group("enemy"):
				collided_object.emit_signal("area_entered", self) # Emit a signal to the Enemy scene
