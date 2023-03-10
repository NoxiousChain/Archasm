extends KinematicBody2D

var speed = 200
var motion : Vector2 = Vector2.ZERO
var up : Vector2 = Vector2.UP
var gravity = 600
var move_speed : float = 0
export var max_move_speed = 250

var vspeed : float = 0
var hspeed : float = 0

onready var sprite : Sprite = get_node("Sprite")

var touching_ground : bool = false
var direction = 1 #1 = right, -1 = left

var coy_time : bool = false
onready var coy_timer = $coyote_timer


func _physics_process(delta: float) -> void:
	quick_move(delta)
	apply_physics(delta)

func apply_physics(delta : float) -> void:

	if(is_on_ceiling()):
		motion.y = 10
		vspeed = 10
	
	if(touching_ground == true and !is_on_floor()): # just left the ground
		coy_time = true
		coy_timer.start(0.2)
		
	if(is_on_floor()):
		touching_ground = true
		coy_timer.stop()
		coy_time = false
	else:
		touching_ground = false

	if(!is_on_floor()):
		vspeed += (gravity * delta)
	else:
		vspeed = 0
	motion.y = vspeed
	motion.x = hspeed

	motion = move_and_slide(motion,up)


func quick_move(var delta : float) -> void:

	if(is_on_wall()):
		move_speed = 0

	if(direction == 1):
		move_speed += speed * delta
		if(direction == -1):
			move_speed = 0
	elif(direction == -1):
		move_speed -= speed * delta
		if(direction == 1):
			move_speed = 0
	else:
		move_speed = lerp(move_speed,0,0.5)
		
	move_speed = clamp(move_speed,-max_move_speed,max_move_speed)
	
	hspeed = move_speed
	if motion.x < 0:
		sprite.flip_h = true
	if motion.x > 0:
		sprite.flip_h = false

