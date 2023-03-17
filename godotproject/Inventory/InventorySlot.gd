tool
extends Panel

var itemSprite = null
export var defaultStyle = preload("res://resources/Inventory/test-inventory-slot.png")
export var emptyStyle = preload("res://resources/Inventory/test-inventory-slot.png")

func _ready():
	pass

func refresh_style():
	if (itemSprite == null):
		set("custom_styles/panel", emptyStyle)
	else:
		set("custom_styles/panel", defaultStyle)

func full() -> bool:
	return itemSprite == null
	
func getItem():
	return itemSprite

func take_from_slot():
	remove_child(itemSprite)
	var copy = itemSprite
	itemSprite = null
	refresh_style()
	return copy
	
func put_in_slot(item):
	itemSprite = item
	add_child(item)
	item.set_position(Vector2(0,0))
	refresh_style()
