#include "InventorySlot.hpp"

void InventorySlot::_init()
{
	itemSprite = nullptr;
	loader = ResourceLoader::get_singleton();
	defaultStyle = StyleBoxTexture::_new();
	emptyStyle = StyleBoxTexture::_new();
	defaultStyle->set_texture((Ref<Texture>)loader->load("res://resources/inventory/item_slot_default.png"));
	emptyStyle->set_texture((Ref<Texture>)loader->load("res://resources/inventory/item_slot_empty.png"));
}

void InventorySlot::_register_methods()
{
	register_method("take_from_slot", &InventorySlot::take_from_slot);
	register_method("put_in_slot", &InventorySlot::put_in_slot);
	register_method("refresh_style", &InventorySlot::refresh_style);
}

void InventorySlot::_ready()
{
}

void InventorySlot::refresh_style()
{
	if (itemSprite == nullptr)
		set("custom_styles/panel", *emptyStyle);
	else
		set("custom_styles/panel", *defaultStyle);
}

ItemSprite* InventorySlot::take_from_slot()
{
	remove_child(item);
	ItemSprite* copy = itemSprite;
	itemSprite = nullptr;
	Godot::print(copy == nullptr ? "take_from_slot() returning nullptr" : "take_from_slot() functioning properly");
	refresh_style();
	return copy;
}

void InventorySlot::put_in_slot(ItemSprite* item)
{
	itemSprite = item;
	item->set_position({ 0, 0 });
	add_child(item);
	refresh_style();
}