extends Control


func _input(_event):
	if Input.is_action_pressed("ui_cancel"):
		var _err = get_tree().change_scene("res://scenes/Menu.tscn")
