  # Basic definition for a weapon class. Needs implementation of most functions
class_name Weapon
extends Item

enum Rarity {
	COMMON,
	UNUSUAL,
	EXCEPTIONAL,
	ANCIENT,
	MYTHIC,
	LEGENDARY
}

# Effects is a bitflag, so you can check if it has a certain value by using the bitwise & operator
# Ex: if effects & Effects.BLEED: # would check if the weapon has the bleed effect
# Use | to set bits (e.g. effects |= Effects.LIGHT)
enum Effects {
	LIGHT = 1,
	HEAVY = 2,
	BLEED = 4,
	QUICK = 8,
	STUN = 16,
	DEFENSE = 32
}

var rarity : int
export(int, FLAGS, "Light,Heavy,Bleed,Quick,Stun,Defense") var effects = 0
export(float) var attack

func _ready():
	pass

func get_damage() -> float:
	return attack # do more advanced damage calculations here
	
func gen_item_description(description_container: MarginContainer, item_description: ItemDescription):
	gen_description_container(description_container)

func on_use():
	pass
	
func on_alt_use():
	pass

func on_attack():
	pass

func on_alt_attack():
	pass
