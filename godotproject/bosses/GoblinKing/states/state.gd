extends Node2D
class_name State

var change_state
var animation
var persistent_state
var velocity = 0

func setup(change_state, animation, persistent_state):
	self.change_state = change_state
	self.animation = animation
	self.persistent_state = persistent_state

func _ready():
	pass
