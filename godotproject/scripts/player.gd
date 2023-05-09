extends KinematicBody2D
class_name Player

export var gravity = 600
export var acc = 350
export var max_move_speed = 250
export var jump_height = 300

var vspeed : float = 0
var hspeed : float = 0
var move_speed : float = 0
var last_chunk : int = 0

var test_damage = 40

var touching_ground : bool = false
var coy_time : bool = false # this will let us do coytoee time
var motion : Vector2 = Vector2.ZERO
var up : Vector2 = Vector2.UP

var enemy_scene = preload("res://scenes/GOBLIN.tscn")

onready var animation = $AnimationPlayer
onready var sprite : Sprite = get_node("Sprite")
onready var coy_timer = $coyote_timer
onready var chunk_manager = get_parent().get_parent().get_node("ChunkManager").get_child(0)

signal stats_loaded

onready var hud = get_node("/root/World/HUD")
onready var inventory = get_node("/root/World/HUD/Inventory")

# Dictionary that contains all player stats
# Data gets loaded from player_stats.tres
# If we add better save options this will need to be expanded
onready var stats_resource = preload("res://Player/player_stats.tres")
var player_stats = {}

signal health_updated(health, max_health, animation)
signal killed()

signal moved_chunks(dir)

func _ready():
	position.y = -1000
	load_stats()
	var _err = connect("stats_loaded", inventory, "update_stats_and_name")
	_err = connect("health_updated", hud, "update_healthbar")
	_err = connect("health_updated", inventory, "update_health")
	_err = connect("moved_chunks", get_node("/root/World"), "update_chunks")
	emit_signal("stats_loaded", self)
	
	last_chunk = chunk_manager.world_to_chunk_x(position.x)
	
func _physics_process(delta: float) -> void:
	quick_move(delta)
	apply_physics(delta)
	var chunkX = chunk_manager.world_to_chunk_x(position.x)
	var diff = chunkX - last_chunk
	if diff == 0:
		last_chunk = chunkX
		return
	emit_signal("moved_chunks", bool(diff > 0))
	last_chunk = chunkX

func apply_physics(delta : float) -> void:

	if(!is_on_floor()):
		vspeed += (gravity * delta)
		animation.play("jump_air")
	else:
		vspeed = 0
		if(Input.is_action_just_pressed("move_jump")):
			vspeed = -jump_height

	motion.y = vspeed
	motion.x = hspeed

	motion = move_and_slide(motion,up)


func quick_move(var delta : float) -> void:
	if(Input.is_action_pressed("move_right")):
		animation.play("run")
		move_speed += acc * delta
		if(Input.is_action_pressed("move_left")):
			move_speed = 0
	elif(Input.is_action_pressed("move_left")):
		animation.play("run")
		move_speed -= acc * delta
		if(Input.is_action_pressed("move_right")):
			move_speed = 0
	else:
		animation.play("idle")
		
	move_speed = clamp(move_speed,-max_move_speed,max_move_speed)
	
	hspeed = move_speed
	if motion.x < 0:
		sprite.flip_h = true
	if motion.x > 0:
		sprite.flip_h = false

func getPosition() -> Vector2:
	return position

# PLAYER STATS
# Any new stats that are created must be added to all 4 of these functions
# ----------------------------------------------------
func load_stats():
	player_stats["name"] = stats_resource.name
	player_stats["max_health"] = stats_resource.max_health
	player_stats["health"] = stats_resource.health
	player_stats["attack"] = stats_resource.attack
	player_stats["defense"] = stats_resource.defense
	player_stats["speed_multi"] = stats_resource.speed_multi
	emit_signal("stats_loaded", self)

func get_stat(key):
	return player_stats.get(key)
	
func set_stat(key, value):
	if key in player_stats:
		player_stats[key] = value

func save_stats():
	stats_resource.name = player_stats["name"]
	stats_resource.max_health = player_stats["max_health"]
	stats_resource.health = player_stats["health"]
	stats_resource.attack = player_stats["attack"]
	stats_resource.defense = player_stats["defense"]
	stats_resource.speed_multi = player_stats["speed_multi"]

	# Save updated resource file
	ResourceSaver.save("res://Player/player_stats.tres", stats_resource)
# ----------------------------------------------------

# Some methods for getting common stats
func get_name() -> String:
	return get_stat("name")
func get_max_health() -> int:
	return get_stat("max_health")
func get_health() -> int:
	return get_stat("health")
func get_attack() -> float:
	return get_stat("attack")
func get_defense() -> int:
	return get_stat("defense")


func damage(amount):
	# try to save some computation time
	if amount == 0: 
		return
	# restrict to range(0,max_health)
	var max_health = get_max_health()
	var health = clamp(get_health() - amount, 0.0, max_health)
	set_stat("health", health)
	emit_signal("health_updated", health, max_health, 0)
	if health == 0:
		kill()
		emit_signal("killed")
		
func heal(amount):
	# try to save some computation time
	if amount == 0: 
		return
	# restrict to range(0,max_health)
	var max_health = get_max_health()
	var health = clamp(get_health() + amount, 0, max_health)
	set_stat("health", health)
	emit_signal("health_updated", health, max_health, 0)
	 
func kill():
	pass
	
func set_health(val)                                                                                                                                                                                                                                                                                                     :
	var prev_health = get_health()
	var max_health = get_max_health()
	var health = clamp(val, 0, max_health) # make sure new health doesn't exceed max health
	if health != prev_health:
		set_stat("health", health)	# update actual health stat
		emit_signal("health_updated", float(health), float(max_health), 0)
		if health == 0:
			kill()
			emit_signal("killed")

func _input(event):
	# Check if the left mouse button was pressed
	if event is InputEventMouseButton and event.button_index == BUTTON_LEFT and event.pressed:
		# Get the mouse position
		var mouse_pos = get_global_mouse_position()

		# Cast a ray from the mouse position to detect if it hits an enemy
		var space_state = get_world_2d().direct_space_state
		var result = space_state.intersect_ray(get_global_transform().origin, mouse_pos, [], 2)

		# Check if the ray hits an enemy
		if result:
			var collided_object = result["collider"]
			if collided_object.is_in_group("enemy"):
				# Call the apply_damage function on the enemy
				collided_object.apply_damage(40)
