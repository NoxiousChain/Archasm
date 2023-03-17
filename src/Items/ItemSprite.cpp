#include "ItemSprite.hpp"

ItemSprite::~ItemSprite()
{
	description_container->queue_free();
	description_container = nullptr;
	item_description->queue_free();
	item_description = nullptr;
}

void ItemSprite::_init()
{
	Godot::print("ItemSprite::_init() called");

	item = Ref<Item>();
	description_container = nullptr;
	item_description = nullptr;
}

void ItemSprite::_register_methods()
{
	Godot::print("ItemSprite::_register_methods() called");

	register_method("_ready", &ItemSprite::_ready);
	register_method("showDescription", &ItemSprite::showDescription);
	register_method("_setItem", &ItemSprite::_setItem);
	register_method("_getItem", &ItemSprite::_getItem);

	register_property<ItemSprite, Ref<Item>>("item", &ItemSprite::_setItem, &ItemSprite::_getItem, Ref<Item>());
}

void ItemSprite::_ready()
{
	Godot::print("ItemSprite::_ready() called");

	item_description = ItemDescription::_new();
	description_container = MarginContainer::_new();
	add_child(description_container);
	description_container->set_owner(this);
	description_container->add_child(item_description);
	item_description->set_owner(description_container);
	if (item != nullptr) {
		set_texture(item->getTexture());
		item->genItemDescription(description_container, item_description);
	}
}

void ItemSprite::_setItem(Ref<Item> other)
{
	item = other;
	set_texture(item->getTexture());
}

Ref<Item> ItemSprite::_getItem() const
{
	return item;
}

void ItemSprite::showDescription(bool visible)
{
	if (item_description != nullptr) {
		// Draw background, etc
		item_description->set_visible(visible);
	}
	else ERR_PRINT("Item description does not exist!");
}