extends KinematicBody2D

var speed = 170
var jump_power = 200
const gravity = 600
var velocity = Vector2.ZERO

onready var player = get_node("../DayNightCycleForeground/player")
onready var sprite : Sprite = get_node("goblin")
onready var animation = $AnimationPlayer

# Add a timer to control the spear shooting
onready var spearTimer = Timer.new()
onready var spearScene = preload("../Scenes/spear.tscn")

var isSpearShot = false  # New flag to track if a spear has been shot recently

func _ready():
	animation.play("gobAnim")
	spearTimer.wait_time = 1  # Set the wait time for the spear timer

	# Connect the timer's timeout signal to a callback function
	spearTimer.connect("timeout", self, "_on_spear_timer_timeout")

	# Start the timer
	add_child(spearTimer)
	spearTimer.start()

func _physics_process(delta):
	var direction = (player.get_global_position() - get_global_position()).normalized()

	# Check if the goblin is within a certain range of the player
	if get_global_position().distance_to(player.get_global_position()) < 200:
		velocity.x = 0  # Stop moving horizontally

		# Shoot a spear if the timer is stopped and a spear has not been shot recently
		if spearTimer.is_stopped() and not isSpearShot:
			shoot_spear()
			isSpearShot = true  # Set the flag to true after shooting a spear

	else:
		velocity.x = direction.x * speed  # Resume horizontal movement

	velocity.y += gravity * delta
	velocity = move_and_slide(velocity, Vector2.UP)

	if is_on_wall():
		if is_on_floor():
			velocity.y = -jump_power

	if direction.x < 0:
		sprite.flip_h = true
	if direction.x > 0:
		sprite.flip_h = false

func shoot_spear():
	var spearInstance = spearScene.instance()  # Create a new instance of the spear scene
	spearInstance.position = global_position  # Set the position of the spear instance to the goblin's position
	spearInstance.look_at(player.global_position)  # Look at the player's position
	get_parent().add_child(spearInstance)

	# Reset and start the timer
	spearTimer.stop()
	spearTimer.start()

func _on_spear_timer_timeout():
	shoot_spear()  # Shoot a spear when the timer times out
	isSpearShot = false  # Set the flag to false after the timer times out
