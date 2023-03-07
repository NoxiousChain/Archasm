#pragma once

#include "../Common.hpp"
#include <TextureRect.hpp>
#include <Resource.hpp>

// Class designed to be used as a scene to create a sprite for an item.
// This sprite should be used for the inventory 

namespace godot {

class ItemSprite : public TextureRect
{
	GODOT_CLASS(ItemSprite, TextureRect);

public:
	Ref<Resource> item;

	void _init();
	static void _register_methods();

	// getters and setters use godot std _ prefix
	void _setItem(Ref<Resource> other);
	Ref<Resource> _getItem();
};

}