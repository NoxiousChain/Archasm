class_name Materials
extends Item



func ready():
	pass
	
func gen_item_description(description_container: MarginContainer, item_description: ItemDescription):
		gen_description_container(description_container)

# You can probably leave these empty, or make them deal a bit of damage
func on_use():
	pass
	
func on_alt_use():
	pass
