# Basic definition for a armor class. Needs a lot more.
class_name Armor
extends Item

export(float) var protection
export(float) var poison_protection
export(float) var fire_protection

func _ready():
	pass

func get_protection() -> float:
	return protection
	
func gen_item_description(description_container: MarginContainer, item_description: ItemDescription):
	gen_description_container(description_container)

# You can probably leave these empty, or make them deal a bit of damage
func on_use():
	pass
	
func on_alt_use():
	pass
