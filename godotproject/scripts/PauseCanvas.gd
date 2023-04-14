extends CanvasLayer

onready var continueButton = $Continue
onready var quitButton = $MainM
onready var texRect = $TextureRect
onready var pLabel = $PauseLabel
func _ready() -> void:
	quitButton.visible = false
	continueButton.visible = false
	texRect.visible = false
	pLabel.visible= false
	pass

func _input(event: InputEvent) -> void:
	if event.is_action_pressed("ui_cancel"):
		get_tree().paused = !get_tree().paused
		quitButton.visible = true
		continueButton.visible = true
		texRect.visible = true
		pLabel.visible = true
		pass


func _on_Continue_pressed() -> void:
	get_tree().paused = false
	quitButton.visible = false
	continueButton.visible = false
	texRect.visible = false
	pLabel.visible= false
	pass # Replace with function body.


func _on_MainM_pressed():
	quitButton.visible = false
	continueButton.visible = false
	texRect.visible = false
	pLabel.visible= false
	get_tree().paused = false
	get_tree().change_scene("res://scenes/Menu.tscn")
	pass # Replace with function body.
