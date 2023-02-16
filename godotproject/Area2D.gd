extends Area2D

export(NodePath) var tilemap_path
export(float) var swing_time = 0.25

var is_swinging = true

var tilemap : TileMap

onready var swing_timer := $Timer
onready var ani_player := $AnimationPlayer

func _ready():
	tilemap = get_node(tilemap_path)
	
func _physics_process(_delta: float) -> void:
	look_at(get_global_mouse_position())
	
	if(Input.is_action_pressed("mb_left")):
		start_swing()
	if(Input.is_action_pressed("mb_right")):
		stop_swing()
		
func start_swing():
	is_swinging = true
	swing_timer.start(swing_time)
	ani_player.play("swing")
	ani_player.playback_speed = 1/swing_time
	
func stop_swing():
	is_swinging = false
	swing_timer.stop()
	ani_player.stop()
	$sprite.scale = Vector2(1,1)
	
func _swing_timer_timeout() -> void:
	hit_block()
	
		
		
