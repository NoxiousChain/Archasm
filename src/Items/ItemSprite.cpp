#include "ItemSprite.hpp"

void ItemSprite::_init()
{

}

void ItemSprite::_register_methods()
{
	register_property<ItemSprite, Ref<Resource>>("item", &ItemSprite::item, Ref<Resource>());
}

void ItemSprite::_setItem(Ref<Resource> other)
{
	item = other;
	set_texture(other->get_texture());
}

Ref<Resource> godot::ItemSprite::_getItem()
{
	return item;
}
