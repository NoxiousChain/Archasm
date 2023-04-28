extends Node2D

const chunk_length = 500 #WIP not working on it right now

signal toggle_interact

onready var inventory = $HUD/Inventory
onready var player = $DayNightCycleForeground/player
onready var cm = $ChunkManager.get_child(0)

var debug_toggle = false

func _ready():
	$AnimationPlayer.play("DayNight_cycle")
	get_node("ParallaxBackground/AnimationPlayer").play("DayNightBack_cycle")

	var screen_width = int(get_viewport_rect().size.x)
	cm.set_save_name("testsave1")
	#cm.load_chunk_at_x(int(player.getPosition().x))
	cm.initialize("testsave1", int(player.getPosition().x), screen_width)

#	var rand = RandomNumberGenerator.new()
#	var timer := Timer.new()
#	add_child(timer)
#
#
#	var waitToSpawn = rand.randf_range(5, 10)
#	timer.wait_time = waitToSpawn
#	timer.start()
#
#	# Connect signals
#	var _err = timer.connect("timeout", self, "_on_timer_timeout")
#	_err = connect("toggle_interact", $TileMap, "_toggle_interact")
#
#func _on_timer_timeout() -> void:
#	var goblin = load("res://scenes/Goblin.tscn")
#	var voidling = load("res://scenes/voidling.tscn")
#	var rand = RandomNumberGenerator.new()
#	var screen_size = get_viewport().get_visible_rect().size
#
#	var enemy = goblin.instance()
#
#	rand.randomize()
#
#	var x = rand.randf_range(0, screen_size.x)
#	enemy.position.y = (-screen_size.y/2)+325
#	enemy.position.x = x
#	add_child(enemy)
	
func update_chunks(dir: bool):
	cm.load_chunk(dir)
	cm.save_chunk(!dir)
	cm.delete_chunk(!dir)
	
func toggle_inventory():
	var visible = inventory.visible
	if visible:
		inventory.hide()
	else:
		inventory.show()
	inventory.set_process_input(!visible)

func _input(_event):
	if Input.is_action_just_pressed("ui_focus_next"):
		toggle_inventory()
		emit_signal("toggle_interact")
	elif Input.is_action_pressed("damage_player"):
		player.damage(1)
	elif Input.is_action_pressed("heal_player"):
		player.heal(1)
	elif Input.is_action_pressed("debug"):
		if !debug_toggle:
			$HUD/HealthBar_Bar.call("set_color", Color.green)
		else:
			$HUD/HealthBar_Bar.call("set_color")
		debug_toggle = !debug_toggle
