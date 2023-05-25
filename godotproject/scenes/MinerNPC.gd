extends KinematicBody2D

export var speed = 100
var jump_height = 50 
export var gravity = 300
export var move_time = 2
export var pause_time = 5

onready var sprite = $Sprite

var velocity = Vector2.ZERO
var direction = Vector2.RIGHT # starting direction
var move_timer = move_time # start moving
var pause_timer = 0

func _ready():
	set_process(true)

func _physics_process(delta):
	if pause_timer > 0: # if currently paused
		pause_timer -= delta
		if pause_timer <= 0: # if pause time is up, start moving again
			move_timer = move_time
	else: # if currently moving
		if move_timer > 0: # if still time to move in this direction
			velocity.y += gravity * delta
			velocity.x = direction.x * speed
			velocity = move_and_slide(velocity, Vector2.UP)
			move_timer -= delta
			if is_on_wall(): # if hits a wall
				move_timer = move_time # start new move timer
				jump() # jump off wall
		else: # if time to pause
			velocity.x = 0
			pause_timer = pause_time
			direction *= -1 # flip direction

	if direction.x < 0:
		sprite.flip_h = true
	if direction.x > 0:
		sprite.flip_h = false

func jump():
	velocity.y = -sqrt(2 * gravity * jump_height)
