class_name ItemSprite
extends TextureRect

export(Resource) var item_res setget set_item, get_item
var item : Item
var description_container : MarginContainer
var item_description : ItemDescription


onready var myLabel = $Label

func _ready():
	var rand_val = randi() % 3
	if rand_val == 0:
		item.item_name = "Wooden Sword"
		
	item.quantity = randi() % item.stack_size + 1
		
	if item.stack_size == 1:
		$Label.visible = false
	else:
		$Label.text = String(item.quantity)
	
	item_description = ItemDescription.new()
	description_container = MarginContainer.new()
	add_child(description_container)
	description_container.set_owner(self)
	description_container.add_child(item_description)
	item_description.set_owner(description_container)
	if item != null:
		set_texture(item.texture)
		item.gen_item_description(description_container, item_description)
		myLabel.text = str(item.quantity)
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

func add_item_quantity(amount_to_add):
	item.quantity += amount_to_add
	$Label.text = String(item.quantity)

func decrease_item_quantity(amount_to_remove):
	item.quantity -= amount_to_remove
	$Label.text = String(item.quantity)

func set_items(nm, qt):

	set_item(Item)
	
	if item.stack_size == 1:
		$Label.visible = false
	else:
		$Label.visible = true
		$Label.text = String(item.quantity)
