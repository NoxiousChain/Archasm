extends Node2D
class_name State

var change_state
var animation
var persistent_state
var velocity = 0

func setup(_change_state, _animation, _persistent_state):
	self.change_state = _change_state
	self.animation = _animation
	self.persistent_state = _persistent_state

func _ready():
	pass
