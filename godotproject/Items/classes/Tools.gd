class_name Tools
extends Item

export (int)var damage
export (int)var durability
export (int)var efficency


func _ready():
	pass

func gen_item_description(description_container: MarginContainer, item_description: ItemDescription):
		gen_description_container(description_container)
