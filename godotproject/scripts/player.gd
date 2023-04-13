extends KinematicBody2D

export var gravity = 600
export var acc = 350
export var max_move_speed = 250
export var jump_height = 300

var vspeed : float = 0
var hspeed : float = 0
var move_speed : float = 0

var touching_ground : bool = false
var coy_time : bool = false # this will let us do coytoee time
var motion : Vector2 = Vector2.ZERO
var up : Vector2 = Vector2.UP

onready var animation = $AnimationPlayer
onready var sprite : Sprite = get_node("Sprite")
onready var coy_timer = $coyote_timer

# Status, health, etc. variables and signals
# setget methods are only called when referenced outside the class - if you need functionality, call self.var instead of just var
export (float) var max_health = 100 
onready var health = max_health setget set_health # just a setter

signal health_updated(health)
signal killed()

func _ready():
	pass
	
func _physics_process(delta: float) -> void:
	quick_move(delta)
	apply_physics(delta)

func apply_physics(delta : float) -> void:

	if(is_on_ceiling()):
		motion.y = 10
		vspeed = 10
	
	if(touching_ground == true and !is_on_floor()): # just left the ground
		animation.play("idle")
		coy_time = true
		coy_timer.start(0.2)
		
	if(is_on_floor()):
		touching_ground = true
		coy_timer.stop()
		coy_time = false
	else:
		touching_ground = false
		animation.play("idle")

	if(!is_on_floor()):
		vspeed += (gravity * delta)
		animation.play("jump_liftOff")
		animation.play("jump_air")
	else:
		vspeed = 0
		if(Input.is_action_just_pressed("move_jump")):
			
			coy_time = false
			coy_timer.stop()
			vspeed = -jump_height
			
	if(coy_time and Input.is_action_just_pressed("move_jump")):
		coy_time = false
		coy_timer.stop()
		vspeed = -jump_height

	motion.y = vspeed
	motion.x = hspeed

	motion = move_and_slide(motion,up)


func quick_move(var delta : float) -> void:

	if(is_on_wall()):
		move_speed = 0

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
		move_speed = lerp(move_speed,0,0.5)
		
	move_speed = clamp(move_speed,-max_move_speed,max_move_speed)
	
	hspeed = move_speed
	if motion.x < 0:
		sprite.flip_h = true
	if motion.x > 0:
		sprite.flip_h = false

func _on_coyatoe_timer_timeout() -> void:
	coy_time = false
	
func getPosition() -> Vector2:
	return position


func damage(amount):
	set_health(health - amount)
	 
func kill():
	pass
	
func set_health(val) -> void:
	var prev_health = health
	health = clamp(val, 0, max_health)
	if health != prev_health:
		emit_signal("health_updated", health)
		if health == 0:
			kill()
			emit_signal("killed")
