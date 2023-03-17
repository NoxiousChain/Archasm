extends Node2D

onready var gridContainer = $GridContainer
onready var bgTexture = $TextureRect
onready var sscale = $GridContainer.rect_scale
var heldItem: TextureRect = null
const SlotClass = preload("res://Inventory/Slot.gd")
var offset = 0

func _ready():
	add_to_group("Persist")
	for slot in gridContainer.get_children():
		slot.connect("gui_input", self, "slot_gui_input", [slot])
		
func slot_gui_input(event: InputEvent, slot: SlotClass):
	if event is InputEventMouseButton:
		if event.button_index == BUTTON_LEFT && event.pressed:
			# If holding an item
			if heldItem != null:
				# If full, swap held item with slot item
				if slot.full():
					var copy = slot.take_from_slot()
					offset = event.position*sscale
					offset = offset.limit_length(42.0)
					copy.set_global_position(get_global_mouse_position() - offset)
					heldItem.expand = false
					remove_child(heldItem)
					slot.put_in_slot(heldItem)
					heldItem = copy
					heldItem.set_scale(sscale)
					add_child(heldItem)
				else: # Just put held item into slot
					remove_child(heldItem)
					slot.put_in_slot(heldItem)
					heldItem = null
			# If not holding anything and slot is full
			elif slot.full():
				heldItem = slot.take_from_slot()
				heldItem.set_scale(sscale)
				add_child(heldItem)
				offset = event.position*sscale
				offset = offset.limit_length(42.0)
				heldItem.set_global_position(get_global_mouse_position() - offset)
		elif event.button_index == BUTTON_RIGHT && event.pressed:
			if !slot.full():
				var scene: PackedScene = load("res://Inventory/Test_Item.tscn")
				var instance = scene.instance()
				slot.put_in_slot(instance)
			else:
				slot.take_from_slot()
	
func _input(event):
	if heldItem:
		heldItem.set_global_position(get_global_mouse_position() - offset)
