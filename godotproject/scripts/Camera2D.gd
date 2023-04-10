extends Camera2D


var spd = 20

func _physics_process(_delta: float) -> void:
	if(Input.is_action_pressed("ui_right")):
		global_position.x += spd
	if(Input.is_action_pressed("ui_left")):
		global_position.x -= spd
	if(Input.is_action_pressed("ui_up")):
		global_position.y -= spd
	if(Input.is_action_pressed("ui_down")):
		global_position.y += spd
