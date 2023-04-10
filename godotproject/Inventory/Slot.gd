extends Panel

var defaultTexture = preload("res://resources/inventory/inventory_slot.png")

var defaultStyle: StyleBoxTexture = null

var item: TextureRect = null

func _ready():
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
