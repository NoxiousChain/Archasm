extends KinematicBody2D

var speed = 20000
var motion : Vector2
var gravity = 100
var move_speed : float = 0
export var max_move_speed = 35
onready var player = get_node("../DayNightCycleForeground/player")


var vspeed : float = 0

#var screen_size = get_viewport().get_visible_rect().size
var screen_size = get_viewport_rect().size

onready var sprite : Sprite = get_node("Sprite")

var touching_ground : bool = false
var direction = 1 # 1 = right, -1 = left

func _physics_process(delta: float) -> void:
	movement(delta)
	apply_physics(delta)


func apply_physics(delta : float) -> void:
	vspeed += (gravity * delta)
	motion.y = vspeed
	motion.x = move_speed
	
	motion = move_and_slide(motion)
	

func movement(var delta : float) -> void:
	if position.x < (player.getPosition().x):    
		direction = 1
	if position.x > (player.getPosition().x):
		direction = -1

	move_speed += speed * delta * direction
		
	move_speed = clamp(move_speed,-max_move_speed,max_move_speed)
	
	if(is_on_wall()):
		position.y = position.y-20
