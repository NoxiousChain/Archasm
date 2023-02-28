extends Control


# Declare member variables here. Examples:
# var a = 2
# var b = "text"


# Called when the node enters the scene tree for the first time.
func _ready():
	$VBoxContainer/Startbutton.grab_focus()

func _on_Startbutton_pressed():
	get_tree().change_scene("res://World.tscn")
