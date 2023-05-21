extends Node

const SlotClass = preload("res://Inventory/Slot.gd")
const ItemClass = preload("res://Items/classes/ItemSprite.gd")
const NUM_INVENTORY_SLOTS = 36

var item : Item

var inventory = {
}

#func add_item(item_name, item_quantity):
#	for item in inventory:
#		if inventory[item][0] == item_name:
#			var stack_size = item.stack_size
#			var able_to_add = stack_size - inventory[item][1]
#			if able_to_add >= item_quantity:
#				inventory[item][1] += item_quantity
#				return
#			else:
#				inventory[item][1] += able_to_add
#				item_quantity = item_quantity - able_to_add
#
#	# item doesn't exist in inventory yet, so add it to an empty slot
#	for i in range(NUM_INVENTORY_SLOTS):
#		if inventory.has(i) == false:
#			inventory[i] = [item_name, item_quantity]
#			return
#
#func remove_item(slot: SlotClass):
#	inventory.erase(slot.slot_index)
#
#func add_item_to_empty_slot(item: ItemClass, slot: SlotClass):
#	inventory[slot.slot_index] = [item.item_name, item.item_quantity]
#
#func add_item_quantity(slot: SlotClass, quantity_to_add: int):
#	inventory[slot.slot_index][1] += quantity_to_add
