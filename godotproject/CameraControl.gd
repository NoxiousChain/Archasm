extends Camera2D

onready var player = get_node("/root/Node2D/Player")

func _process(delta):
	position.x = player.position.x
