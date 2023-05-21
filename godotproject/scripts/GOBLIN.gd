extends KinematicBody2D

var speed = 170
var jump_power = 170
const gravity = 600
var velocity = Vector2.ZERO

var goblinLaugh = preload("res://Sounds/goblinLaugh.wav")
var goblinDeath = preload("res://Sounds/goblinDeath.wav")

export(int) var health = 100
var max_health = health
onready var healthBar = $HPBar

onready var player = get_node("../DayNightCycleForeground/player")
onready var sprite : Sprite = get_node("goblin")
onready var animation = $AnimationPlayer

# Add a timer to control the spear shooting
onready var spearTimer = Timer.new()
onready var spearScene = preload("../Scenes/spear.tscn")

var isSpearShot = false  # New flag to track if a spear has been shot recently
var rand = RandomNumberGenerator.new()

func _ready():
	healthBar.visible = false
	spearTimer.wait_time = 2  # Set the wait time for the spear timer

	# Connect the timer's timeout signal to a callback function
	spearTimer.connect("timeout", self, "_on_spear_timer_timeout")

	# Start the timer
	add_child(spearTimer)
	spearTimer.start()

func _physics_process(delta):
	if !is_inside_tree():
		return
	
	var direction = (player.get_global_position() - get_global_position()).normalized()
	
	if not health == 0:
		# Check if the goblin is within a certain range of the player
		if get_global_position().distance_to(player.get_global_position()) < 200:
			velocity.x = 0  # Stop moving horizontally
			animation.play("still")
			if is_on_floor():
				velocity.y = 0  # stop moving up

			# Shoot a spear if the timer is stopped and a spear has not been shot recently
			if spearTimer.is_stopped() and not isSpearShot:
				shoot_spear()
				isSpearShot = true  # Set the flag to true after shooting a spear

		else:
			velocity.x = direction.x * speed
			animation.play("gobAnim")  # Resume horizontal movement
		
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
	get_parent().add_child(spearInstance) # Add it to scene (BEFORE dealing with position, etc.)
	spearInstance.position = global_position  # Set the position of the spear instance to the goblin's position
	spearInstance.look_at(player.global_position)  # Look at the player's position

	# Reset and start the timer
	spearTimer.stop()
	var randTime = rand.randf_range(3, 7)
	spearTimer.wait_time = randTime
	spearTimer.start()

func _on_spear_timer_timeout():
	shoot_spear()  # Shoot a spear when the timer times out
	isSpearShot = false  # Set the flag to false after the timer times out

func apply_damage(damage):
	healthBar.visible = true
	health -= damage
	
	if health <= 0:
		health = 0  # Make sure health does not go below zero
		$HPBar.set_percent_value_int(float(health)/max_health * 100)
		animation.stop()
		animation.play("dying") 
		velocity = Vector2.ZERO
		spearTimer.stop()
		isSpearShot = true
	else:
		$HPBar.set_percent_value_int(float(health)/max_health * 100)
