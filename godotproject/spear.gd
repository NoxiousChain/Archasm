extends KinematicBody2D

export var speed = 500 # Speed of the projectile
export var lifetime = 2.0 # Lifetime of the projectile in seconds

var velocity = Vector2() # Velocity of the projectile
var lifetime_timer = 0.0 # Timer to keep track of projectile's lifetime

func _ready():
	# Set the projectile's initial velocity
	velocity = Vector2(speed, 0).rotated(rotation)

func _physics_process(delta):
	# Update the lifetime timer
	lifetime_timer += delta
	
	# Update the projectile's position based on its velocity
	move_and_collide(velocity * delta)
	
	# If the lifetime timer exceeds the lifetime, remove the projectile
	if lifetime_timer > lifetime:
		queue_free()
