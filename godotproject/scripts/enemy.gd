extends KinematicBody2D

var speed = 170

export(int) var health = 100
var max_health = health

onready var healthBar = $HPBar

onready var player = get_node("../DayNightCycleForeground/player")
onready var sprite : Sprite = get_node("Sprite")
onready var animation = $AnimationPlayer

func _ready():
	healthBar.visible = false
	
func _physics_process(delta):
	var direction = (player.global_position - global_position).normalized()
	var velocity = direction * speed
	var floor_normal = Vector2.UP
	if direction.x < 0:
		sprite.flip_h = true
	if direction.x > 0:
		sprite.flip_h = false

func apply_damage(damage):
	healthBar.visible = true
	health -= damage
	
	if health <= 0:
		health = 0  # Make sure health does not go below zero
		$HPBar.set_percent_value_int(float(health)/max_health * 100)
		queue_free()
	else:
		$HPBar.set_percent_value_int(float(health)/max_health * 100)



