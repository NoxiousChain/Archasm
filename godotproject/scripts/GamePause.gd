extends Control

func _on_Resumegme_pressed():
	var _err = get_tree().change_scene("res://scenes/World.tscn")

func _on_MMbutton_pressed():
	var _err = get_tree().change_scene("res://scenes/Menu.tscn")
