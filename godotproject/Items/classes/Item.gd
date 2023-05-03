
class_name Item
extends Resource
# These are for display purposes in the item description.
# I had a few ideas for how this would look; see ItemDescription.gd
export(String) var name						# The actual item name
export(String) var description				# Base item description
export(String) var meta						# Funny one-liner/interesting text at the bottom.

export(Texture) var texture					# Texture for the item. This is used in inventories.

export(int) var stack_size					# maximum stack size
export(int) var quantity

func _ready():
	pass
	
# This function generates the description container for the hover text
# Needs work. I started putting this together but didn't get very far at all
func gen_description_container(description_container : MarginContainer):
	if description_container == null:
		return
	description_container.set_custom_minimum_size(Vector2(150,250))
	description_container.set_visible(false)
	description_container.add_constant_override("margin_top", 10)
	description_container.add_constant_override("margin_bottom", 10)
	description_container.add_constant_override("margin_left", 10)
	description_container.add_constant_override("margin_right", 10)

# This function generates the description for an item to be used with the hover text
# It's very basic. Needs more work
#func gen_item_description(description_container : MarginContainer, item_description : ItemDescription):
#	gen_description_container(description_container)
#	var build = name + "\n\n" + description
#	if !meta.empty():
#		build += "\n\n" + meta
#	item_description.build(build)

# This function will be called when the user left clicks with the item in their active hand
# Should almost always be overridden in child classes, unless the item has no functionality
func on_use(): 
	pass

# Similar to on_use(), this function is called whenever the player right clicks with the
# item in their active hand
func on_alt_use():
	pass

# This is used for the inventory. Slots will only accept items of their slot type.
# Possible return values:
# DEFAULT (accepts all types), HELMET, CHEST, LEGS, ACCESSORY_UPPER, ACCESSORY_LOWER
func get_slot_type() -> String:
	return "DEFAULT"

func addQuantity(addedQuant : int):
	quantity += addedQuant
	
func getQuantity() -> int:
	return quantity
