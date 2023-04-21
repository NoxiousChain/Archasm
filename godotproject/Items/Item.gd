class Item extends Resource:
	# These are for display purposes in the item description.
	# I had a few ideas for how this would look; see ItemDescription.gd
	export(String) var name			# The actual item name
	export(String) var description	# Base item description
	export(String) var meta			# Funny one-liner/interesting text at the bottom.

	export(Texture) var texture		# Texture for the item. This is used in inventories.

	export(int) var stack_size		# maximum stack size
	export(int) var quantity		# number of items currently in the stack

	func _ready():
		pass
		
	func gen_description_container(description_container : MarginContainer):
		pass
		
	func gen_item_description(description_container : MarginContainer):
		pass
	
	# This function will be called when the user left clicks with the item in their active hand
	# Should almost always be overridden in child classes, unless the item has no functionality
	func on_use():
		pass
	
	# Similar to on_use(), this function is called whenever the player right clicks with the
	# item in their active hand
	func on_alt_use():
		pass
	
	# This is used for the inventory. Slots will only accept items of their slot type.
	func get_slot_type() -> String:
		return "DEFAULT"
