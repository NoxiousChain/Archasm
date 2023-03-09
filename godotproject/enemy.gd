extends KinematicBody2D

const WALK_SPEED = 1000000
const GRAVITY = 600

onready var player = get_node("World/DayNightCycleForeground/player")

func _ready():
	var rng = RandomNumberGenerator.new()
	rng.randomize()
	position.x = rng.randf_range(-100,100)
	position.y = -100
	
func _physics_process(delta):
	if player:
		var direction = (player.position - position).normalized()
		if not is_on_floor():
			direction.y += GRAVITY
		
		move_and_slide(direction * WALK_SPEED)
		
		for i in get_slide_count():
			var collision = get_slide_collision(i)
			# if collision.collider.name == "Player":
			var object = collision.collider
			if object.is_in_group("player"):
				object.die()
