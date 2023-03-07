extends Area2D

export var speed = 2000
export var moveDistance = 200

onready var startX = position.x
onready var targetX = position.x + moveDistance

func spawn():
	var rng = RandomNumberGenerator.new()
	rng.randomize()
	position.x = rng.randf_range(-200,200)
	position.y = rng.randf_range(0, 100)
	
func _process(delta):
	position.x = move_to_x(position.x, targetX, speed*delta)
	if position.x == targetX:
		if targetX == startX:
			targetX = position.x + moveDistance
		else:
			targetX = startX


func move_to_x(currentPos, newPosition, steps):
	var new = currentPos
	if new < newPosition:
		new += steps
		if new > newPosition:
			new = newPosition
	else:
		new -= steps
		if new < newPosition:
			new = newPosition
	return new
