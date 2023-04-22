extends Control

func _ready():
	pass # Replace with function body.

func _on_Startbutton_pressed():
	var _err = get_tree().change_scene("res://scenes/World.tscn")

func _on_Quit_pressed():
	get_tree().quit()
	
func _on_Options_pressed():
	var _err = get_tree().change_scene("res://scenes/PauseMenu.tscn")
