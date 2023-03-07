#pragma once

#include "../Common.hpp"
#include <Panel.hpp>
#include <ResourceLoader.hpp>
#include <StyleBoxTexture.hpp>
#include <Texture.hpp>
#include "../Items/ItemSprite.hpp"

namespace godot {

class InventorySlot : public Panel {
	GODOT_CLASS(InventorySlot, Panel);

private:
	ItemSprite* itemSprite;	
	ResourceLoader* loader;

	StyleBoxTexture *defaultStyle, *emptyStyle;

public:

	void _init();
	static void _register_methods();

	void _ready();
	void refresh_style ();

	// returns whether the slot is already occupied by an item
	bool full();
	// returns a pointer to the item (nullptr if dne)
	ItemSprite* getItem();
	ItemSprite* take_from_slot();
	void put_in_slot(ItemSprite* item);
};

}