extends Node2D


func _ready():
	$AnimationPlayer.play("DayNight_cycle")
	get_node("ParallaxBackground/AnimationPlayer").play("DayNightBack_cycle")

	var rand = RandomNumberGenerator.new()
	var enemyscene = load("res://GOBLIN.tscn")
	
	var screen_size = get_viewport().get_visible_rect().size
	
	for i in range(0,3):
		var enemy = enemyscene.instance()
		rand.randomize()
		var x = rand.randf_range(0, screen_size.x)
		enemy.position.y = -screen_size.y/2
		enemy.position.x = x
		add_child(enemy)
