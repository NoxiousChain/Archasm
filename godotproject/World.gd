extends Node2D


func _ready():
	$AnimationPlayer.play("DayNight_cycle")
	get_node("ParallaxBackground/AnimationPlayer").play("DayNightBack_cycle")
