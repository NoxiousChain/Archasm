extends KinematicBody2D

const speed = 300
const jump_velocity = -400

onready var player = get_node("../DayNightCycleForeground/player")

var velocity = Vector2.ZERO
var gravity = ProjectSettings.get_setting("physics/2d/default_gravity")
var direction = (player.get_global_position() - get_global_position()).normalized()

var state
var state_factory

func _ready():
	state_factory = StateFactory.new()
	Idle()
	
func _physics_process(delta):
	if not is_on_floor():
		velocity.y += gravity * delta
	velocity = move_and_slide(velocity, Vector2.UP)
	
func change_state(new_state_name):
	if state != null:
		state.queue_free()
	state = state_factory.get_state(new_state_name).new()
	state.setup("change_state", $AnimationPlayer, self)
	state.name = "current_state"
	add_child(state)

func Swing():
	change_state("swing")
	state.swing()
	
func Idle():
	change_state("idle")
	state.idle()
