extends Node2D

func _ready():
	$AnimationPlayer.play("Fadein")
	yield(get_tree().create_timer(6), "timeout")
	$AnimationPlayer.play("Fadeout")
	yield(get_tree().create_timer(3), "timeout")
	get_tree().change_scene("res://scenes/Menu.tscn")

func _input(event):
	if Input.is_action_pressed("ui_accept"):
		get_tree().change_scene("res://scenes/Menu.tscn")
