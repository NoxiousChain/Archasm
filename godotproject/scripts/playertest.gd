extends KinematicBody2D

const UP = Vector2(0, -1)

var velocity = Vector2()
var move_speed = 250
var gravity = 980
var jump_velocity = -400
var is_grounded

onready var raycasts = $RayCasts
onready var animation = $AnimationPlayer
onready var sprite : Sprite = get_node("Sprite")

func _physics_process(delta):
	_get_input()
	velocity.y += gravity * delta
	
	velocity = move_and_slide(velocity, UP)
	
	is_grounded = check_is_grounded()
	
func _input(event):
	if event.is_action_pressed("move_jump") && is_grounded:
		velocity.y = jump_velocity
		
func _get_input():
	var move_direction = -int(Input.is_action_pressed("move_left")) + int(Input.is_action_pressed("move_right"))
	velocity.x = lerp(velocity.x, move_speed * move_direction,0.2)
	if velocity.x < 0:
		sprite.flip_h = true
	if velocity.x > 0:
		sprite.flip_h = false
	
		
 
func check_is_grounded():
	for raycast in raycasts.get_children():
		if raycast.is_colliding():
			return true
			
	return false

		
func _ready():
	pass 
