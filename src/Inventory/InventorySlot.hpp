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

	ItemSprite* take_from_slot();
	void put_in_slot(ItemSprite* item);
};

}