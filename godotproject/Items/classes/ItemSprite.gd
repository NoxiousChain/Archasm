class_name ItemSprite
extends TextureRect

export(Resource) var item_res setget set_item, get_item
var item : Item
var description_container : MarginContainer
var item_description : ItemDescription

func _ready():
	item_description = ItemDescription.new()
	description_container = MarginContainer.new()
	add_child(description_container)
	description_container.set_owner(self)
	description_container.add_child(item_description)
	item_description.set_owner(description_container)
	if item != null:
		set_texture(item.texture)
		item.gen_item_description(description_container, item_description)
	pass

func set_item(_item: Resource):
	item = _item as Item
	set_texture(item.texture)

func get_item() -> Item:
	return item as Item

func show_description(visible: bool):
	if item_description != null:
		item_description.set_visible(visible)
	else:
		printerr("Item Description does not exist!")
	
func get_slot_type() -> String:
	return item.get_slot_type()

func destroy():
	description_container.queue_free()
	description_container = null
	item_description.queue_free()
	item_description = null

func addQuantity(addedQuant : int):
	item_res.addQuantity(addedQuant)
