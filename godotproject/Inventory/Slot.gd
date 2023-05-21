extends Panel

var defaultTexture = preload("res://resources/inventory/inventory_slot.png")

var defaultStyle: StyleBoxTexture = null

var itemScene = preload("res://Inventory/Test_Item.tscn")
var item = null

func _ready():
	if randi() % 2 == 0:
		item = itemScene.instance()
		add_child(item)
	defaultStyle = StyleBoxTexture.new()
	defaultStyle.texture = defaultTexture
	refresh_style()

func refresh_style():
	set('custom_styles/panel', defaultStyle)

func full():
	return item != null

func take_from_slot() -> TextureRect:
	if item == null: return null
	remove_child(item)
	var copy = item
	item = null
	refresh_style()
	return copy

func put_in_slot(_item: TextureRect):
	if(_item == null):
		print("inventory slot received null item")
		return
	item = _item
	item.set_scale(Vector2(2,2))
	item.set_position(Vector2(2,2))
	add_child(item)
	refresh_style()

# Generic slots accept all item types. Override if different acceptance criteria is required (see armor slot examples)
func slot_accepts_item(_i) -> bool:
	return true

func initialize_item(item_name, item_quantity):
	if item == null:
		item = Item.instance()
		add_child(item)
		item.set_item(item_name, item_quantity)
	else:
		item.set_item(item_name, item_quantity)
	refresh_style()
