
extends Control

onready var menu_music = get_node("Music")

func _ready():
	pass # Replace with function body.

func _on_Startbutton_pressed():
	if menu_music != null:
		menu_music.stop()
	var _err = get_tree().change_scene("res://scenes/World.tscn")

func _on_Quit_pressed():
	get_tree().quit()
	
func _on_Options_pressed():
	var _err = get_tree().change_scene("res://scenes/ControlsMenu.tscn")
	
	
