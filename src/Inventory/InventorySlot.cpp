#include "InventorySlot.hpp"

InventorySlot::InventorySlot()
{
}

InventorySlot::~InventorySlot()
{
	// itemSprite = nullptr;
	// style.unref();
	// defaultStyleTexture.unref();
	// emptyStyleTexture.unref();
}

void InventorySlot::_init()
{
	Godot::print("InventorySlot::_init() called");

	style = Ref<StyleBoxTexture>();
	defaultStyleTexture = Ref<Texture>();
	emptyStyleTexture = Ref<Texture>();
}

void InventorySlot::_register_methods()
{
	Godot::print("InventorySlot::_register_methods() called");

	register_property<InventorySlot, Ref<Texture>>("defaultStyleTexture", &InventorySlot::_setDefaultStyleTexture, &InventorySlot::_getDefaultStyleTexture, Ref<Texture>());
	register_property<InventorySlot, Ref<Texture>>("emptyStyleTexture", &InventorySlot::_setEmptyStyleTexture, &InventorySlot::_getEmptyStyleTexture, Ref<Texture>());

	register_method("_ready", &InventorySlot::_ready);
	register_method("take_from_slot", &InventorySlot::take_from_slot);
	register_method("put_in_slot", &InventorySlot::put_in_slot);
	register_method("refreshStyle", &InventorySlot::refreshStyle);
	register_method("full", &InventorySlot::full);
	register_method("getItem", &InventorySlot::getItem);
}

void InventorySlot::_ready()
{
	Godot::print("InventorySlot::_ready() called");
	style = get("custom_styles/panel");
	refreshStyle();
}

void InventorySlot::refreshStyle()
{
	if (itemSprite == nullptr)
		style->set_texture(emptyStyleTexture);
	else
		style->set_texture(defaultStyleTexture);
	set("custom_styles/panel", style);
}

Ref<Texture> InventorySlot::_getDefaultStyleTexture() const
{
	return defaultStyleTexture;
}

Ref<Texture> InventorySlot::_getEmptyStyleTexture() const
{
	return emptyStyleTexture;
}

void InventorySlot::_setDefaultStyleTexture(Ref<Texture> other)
{
	defaultStyleTexture = other;
	refreshStyle();
}

void InventorySlot::_setEmptyStyleTexture(Ref<Texture> other)
{
	emptyStyleTexture = other;
	refreshStyle();
}

bool InventorySlot::full()
{
	return itemSprite != nullptr;
}

ItemSprite* InventorySlot::getItem()
{
	return itemSprite;
}

ItemSprite* InventorySlot::take_from_slot()
{
	Godot::print("take_from_slot() called");
	if (itemSprite == nullptr) return nullptr;
	remove_child(itemSprite);
	ItemSprite* copy = itemSprite;
	itemSprite = nullptr;
	refreshStyle();
	return copy;
}

void InventorySlot::put_in_slot(ItemSprite* item)
{
	Godot::print("put_in_slot() called");
	if (item != nullptr) {
		itemSprite = item;
		itemSprite->set_position({ 0,0 });
		add_child(itemSprite);
		refreshStyle();
	}
	else {
		Godot::print("inventory slot received null item");
	}
}