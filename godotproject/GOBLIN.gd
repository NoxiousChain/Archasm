extends KinematicBody2D

var speed = 20000
var motion : Vector2
var gravity = 200
var move_speed : float = 0
export var max_move_speed = 250

var vspeed : float = 0
var hspeed : float = 50

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
	motion.x = hspeed
	
	motion = move_and_slide(motion)


func movement(var delta : float) -> void:
	
	if position.x < screen_size.x:    
		direction = 1
	if position.x > screen_size.x:
		direction = -1

	if(direction == 1):
		move_speed += speed * delta
	elif(direction == -1):
		move_speed -= speed * delta
		
	move_speed = clamp(move_speed,-max_move_speed,max_move_speed)
	
	

