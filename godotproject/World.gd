extends Node2D


func _ready():
	$AnimationPlayer.play("DayNight_cycle")
	get_node("ParallaxBackground/AnimationPlayer").play("DayNightBack_cycle")
	
	var rand = RandomNumberGenerator.new()
	var timer := Timer.new()
	add_child(timer)
	
	
	var waitToSpawn = rand.randf_range(30, 90)
	timer.wait_time = waitToSpawn
	timer.start()
	
	timer.connect("timeout", self, "_on_timer_timeout")

func _on_timer_timeout() -> void:
	
	var enemyscene = load("res://GOBLIN.tscn")
	var rand = RandomNumberGenerator.new()
	var screen_size = get_viewport().get_visible_rect().size
	
	var enemy = enemyscene.instance()
	rand.randomize()
	var x = rand.randf_range(0, screen_size.x)
	enemy.position.y = (-screen_size.y/2)+300
	enemy.position.x = x
	add_child(enemy)
