extends Node

var intro_music = load("res://sounds/squad_game.mp3")


# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.


func play_music():
	$Music.stream = intro_music
	$Music.play()
