extends Node2D

onready var inventory = $Inventory
onready var hotbar = $Hotbar
onready var armorSlots = [$HelmetSlot, $ChestSlot, $LegsSlot, $AccessorySlot1, $AccessorySlot2]
onready var bgTexture = $TextureRect
onready var sscale = $Inventory.rect_scale*2

var item : Item
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
		
func slot_gui_input(event: InputEvent, slot):
	if event is InputEventMouseButton:
		if event.button_index == BUTTON_LEFT && event.is_pressed():
			# If player is holding shift we will do quick switches
			if Input.is_action_pressed("ui_shift") && slot.full():
				var copy = slot.take_from_slot()
				if slot in inventory.get_children():
					# first try to place item in armor slots
					for aSlot in armorSlots:
						# if armor slot is empty and item can go into it, move item to slot
						if !aSlot.full() && aSlot.call("slot_accepts_item", copy):
							aSlot.put_in_slot(copy)
							return
					# place in first open hotbar slot
					for hSlot in hotbar.get_children():
						if !hSlot.full(): # no need for an accept check, hotbar accepts all items
							hSlot.put_in_slot(copy)
							return
					# place in first open inventory slot
					for iSlot in inventory.get_children():
						if !iSlot.full():
							iSlot.put_in_slot(copy)
							return
					
				elif slot in hotbar.get_children():
					# first try to place item in armor slots
					for aSlot in armorSlots:
						# if armor slot is empty and item can go into it, move item to slot
						if !aSlot.full() && aSlot.call("slot_accepts_item", copy):
							aSlot.put_in_slot(copy)
							return
					# place in first open inventory slot
					for iSlot in inventory.get_children():
						if !iSlot.full():
							iSlot.put_in_slot(copy)
							return
					# place in first open hotbar slot
					for hSlot in hotbar.get_children():
						if !hSlot.full(): # no need for an accept check, hotbar accepts all items
							hSlot.put_in_slot(copy)
							return
				else:
					# place in first open inventory slot
					for iSlot in inventory.get_children():
						if !iSlot.full():
							iSlot.put_in_slot(copy)
							return
					# place in first open hotbar slot
					for hSlot in hotbar.get_children():
						if !hSlot.full(): # no need for an accept check, hotbar accepts all items
							hSlot.put_in_slot(copy)
							return
					# place back into armor slot
					for aSlot in armorSlots:
						# if armor slot is empty and item can go into it, move item to slot
						if !aSlot.full() && aSlot.call("slot_accepts_item", copy):
							aSlot.put_in_slot(copy)
							return
			# If player is not holding shift, do regular processing
			else:
				# if holding an item and slot accepts item type
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
#		elif event.button_index == BUTTON_RIGHT && event.is_pressed():
#			if !slot.full():
#				var scene: PackedScene = load("res://Inventory/Test_Item.tscn")
#				var instance = scene.instance()
#				slot.put_in_slot(instance)
#			else:
#				var _empty = slot.take_from_slot()
	
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

# _animation only here so its compatible with update_health signal 
# any calls to this function must already have health clamped
func update_health(health, max_health, _animation):
	$HealthStat.set_text(str(health) + "/" + str(max_health))

func update_stats(player):
	$HealthStat.set_text(str(player.get_health()) + "/" + str(player.get_max_health()))
	$ArmorStat.set_text(str(player.get_defense()))
	$AttackStat.set_text(str(player.get_attack() * 100) + "%")
	
# updates all values using data from player_stats
func update_stats_and_name(player):
	update_stats(player)
	$PlayerLabel.set_text(player.get_name())
