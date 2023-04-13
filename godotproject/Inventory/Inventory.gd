extends Node2D

onready var inventory = $Inventory
onready var hotbar = $Hotbar
onready var armorSlots = [$HelmetSlot, $ChestSlot, $LegsSlot, $AccessorySlot1, $AccessorySlot2]
onready var bgTexture = $TextureRect
onready var sscale = $Inventory.rect_scale*2

var heldItem = null
const SlotClass = preload("res://Inventory/Slot.gd")
var offset = 0
var dragging = false
var drag_start_local_pos = Vector2()
var mouse_held_on_place = false

func _ready():
	add_to_group("Persist")
	for slot in inventory.get_children():
		slot.connect("gui_input", self, "slot_gui_input", [slot])
	for slot in hotbar.get_children():
		slot.connect("gui_input", self, "slot_gui_input", [slot])
	for slot in armorSlots:
		slot.connect("gui_input", self, "slot_gui_input", [slot])
	
	#set_process_input(true)
		
func slot_gui_input(event: InputEvent, slot):
	if event is InputEventMouseButton:
		if event.button_index == BUTTON_LEFT && event.is_pressed():
			# If holding an item and slot accepts item type
			if heldItem != null && slot.call("slot_accepts_item", heldItem):
				# If full, swap held item with slot item
				if slot.full():
					var copy = slot.take_from_slot()
					offset = event.position*sscale
					offset = offset.limit_length(10.0)
					heldItem.expand = false
					remove_child(heldItem)
					slot.put_in_slot(heldItem)
					heldItem = copy
					heldItem.set_scale(sscale)
					heldItem.set_global_position(get_global_mouse_position() - offset)
					add_child(heldItem)
				else: # Just put held item into slot
					remove_child(heldItem)
					slot.put_in_slot(heldItem)
					heldItem = null
					mouse_held_on_place = true
			# If not holding anything and slot is full
			elif slot.full():
				heldItem = slot.take_from_slot()
				heldItem.set_scale(sscale)
				add_child(heldItem)
				offset = event.position*sscale
				offset = offset.limit_length(10.0)
				heldItem.set_global_position(get_global_mouse_position() - offset)
		elif event.button_index == BUTTON_LEFT && !event.is_pressed():
			mouse_held_on_place = false
		elif event.button_index == BUTTON_RIGHT && event.is_pressed():
			if !slot.full():
				var scene: PackedScene = load("res://Inventory/Test_Item.tscn")
				var instance = scene.instance()
				slot.put_in_slot(instance)
			else:
				var _empty = slot.take_from_slot()
	
func _input(event):
	# update position of held item
	if heldItem:
		heldItem.set_global_position(get_global_mouse_position() - offset)
	elif event is InputEventMouseButton:
		if event.button_index == BUTTON_LEFT:
			if event.is_pressed() and !mouse_held_on_place:
				# We can assume that the inventory size will be equal to the size of the texture
				# Node2D doesn't have an easy way to calculate its size so this will do
				var root_rect = Rect2(Vector2(), bgTexture.get_rect().size)
				# If user is clicking within window and isn't modifying a slot, we enable dragging
				if root_rect.has_point(to_local(event.global_position)):
					dragging = true
					drag_start_local_pos = to_local(event.global_position)
					# Capture mouse cursor
					Input.set_mouse_mode(Input.MOUSE_MODE_CONFINED)
					
			# Stop dragging on button release
			else:
				dragging = false
				# Release mouse cursor
				Input.set_mouse_mode(Input.MOUSE_MODE_VISIBLE)
	# update position as user drags window
	elif event is InputEventMouseMotion and dragging and !mouse_held_on_place:
		var local_mouse_pos = to_local(event.global_position)
		var delta = local_mouse_pos - drag_start_local_pos
		#var delta = event.relative
		position = clamp_position_to_viewport(position + delta)
		var new_global_pos = position + drag_start_local_pos
		Input.warp_mouse_position(new_global_pos)

func clamp_position_to_viewport(pos):
	var viewport_rect = get_viewport().get_visible_rect()
	var node_size = bgTexture.get_rect().size
	pos.x = clamp(pos.x, viewport_rect.position.x, viewport_rect.end.x - node_size.x)
	pos.y = clamp(pos.y, viewport_rect.position.y, viewport_rect.end.y - node_size.y)
	return pos

# Handle captured mouse release on exit/focus loss
# This prevents the mouse from bugging out if the user exits or tabs out while dragging the window
func _notification(what):
	if what == NOTIFICATION_WM_FOCUS_OUT or what == NOTIFICATION_WM_QUIT_REQUEST:
		if dragging:
			dragging = false
			Input.set_mouse_mode(Input.MOUSE_MODE_VISIBLE)
