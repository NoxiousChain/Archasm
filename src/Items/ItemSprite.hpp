#pragma once

#include "../Common.hpp"
#include <TextureRect.hpp>
#include <Resource.hpp>
#include <MarginContainer.hpp>
#include "../Items/Item.hpp"
#include "../Text/ItemDescription.hpp"

// Class designed to be used as a scene to create a sprite for an item.
// This sprite should be used for the inventory, and could also probably
// be used when the item is dropped. More functionality will need to be 
// added for that to be possible.

class ItemSprite : public TextureRect
{
	GODOT_CLASS(ItemSprite, TextureRect)

public:
	~ItemSprite();

private:
	Ref<Item> item;
	MarginContainer* description_container;
	ItemDescription* item_description;

public:
	void _init();
	static void _register_methods();
	void _ready();

	// getters/setters
	void _setItem(Ref<Item> other);
	Ref<Item> _getItem() const;

	void showDescription(bool visible = true);
};
