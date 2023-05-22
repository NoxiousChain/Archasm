extends KinematicBody2D

export var speed = 600 # Speed of the projectile
export var arcHeight = 300 # Height of the arc
export var lifetime = 4.0 # Lifetime of the projectile in seconds

var velocity = Vector2() # Velocity of the projectile
var lifetime_timer = 0.0 # Timer to keep track of projectile's lifetime

func _ready():
	# Set the projectile's initial velocity
	#velocity = Vector2(speed, 0).rotated(rotation)
	if is_on_floor():
		queue_free()


func _physics_process(delta):
	# Update the lifetime timer
	lifetime_timer += delta

	# Update the projectile's position based on its velocity
	# with an arc trajectory
	var gravity = Vector2(0, 600) # Gravity affects the arc trajectory
	var delta_pos = velocity * delta
	delta_pos += 0.5 * gravity * delta * delta
	velocity += gravity * delta

	# Update the rotation based on the velocity direction
	if velocity.length_squared() > 0:
		rotation = velocity.angle()
		if velocity.x == 0:
			rotation = 0

# warning-ignore:return_value_discarded
	move_and_collide(delta_pos)

	# If the lifetime timer exceeds the lifetime, remove the projectile
	if lifetime_timer > lifetime:
		queue_free()

