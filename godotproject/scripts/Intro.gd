extends Node2D

func _ready():
	MusicControls.play_music()
	$AnimationPlayer.play("Fadein")
	yield(get_tree().create_timer(6), "timeout")
	$AnimationPlayer.play("Fadeout")
	yield(get_tree().create_timer(3), "timeout")
	var _err = get_tree().change_scene("res://scenes/Menu.tscn")

func _input(_event):
	if Input.is_action_pressed("ui_accept"):
		var _err = get_tree().change_scene("res://scenes/Menu.tscn")


