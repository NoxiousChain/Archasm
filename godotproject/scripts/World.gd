extends Node2D

signal toggle_interact

func _ready():
	$AnimationPlayer.play("DayNight_cycle")
	get_node("ParallaxBackground/AnimationPlayer").play("DayNightBack_cycle")
	
	var rand = RandomNumberGenerator.new()
	var timer := Timer.new()
	add_child(timer)
	
	
	var waitToSpawn = rand.randf_range(5, 10)
	timer.wait_time = waitToSpawn
	timer.start()
	
	timer.connect("timeout", self, "_on_timer_timeout")
	connect("toggle_interact", $TileMap, "_toggle_interact")

func _on_timer_timeout() -> void:
	
	var enemyscene = load("res://scenes/GOBLIN.tscn")
	var rand = RandomNumberGenerator.new()
	var screen_size = get_viewport().get_visible_rect().size
	
	var enemy = enemyscene.instance()
	rand.randomize()
	var x = rand.randf_range(0, screen_size.x)
	enemy.position.y = (-screen_size.y/2)+325
	enemy.position.x = x
	add_child(enemy)
	
func toggle_inventory():
	var visible = $HUD/Inventory.visible
	if visible:
		$HUD/Inventory.hide()
	else:
		$HUD/Inventory.show()
	$HUD/Inventory.set_process_input(!visible)

func _input(event):
	if Input.is_action_just_pressed("ui_focus_next"):
		toggle_inventory()
		emit_signal("toggle_interact")
