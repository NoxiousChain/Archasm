#pragma once

#include "../Common.hpp"
#include <Node2D.hpp>
#include <Ref.hpp>
#include <Texture.hpp>
#include "../Text/ItemDescription.hpp"

/*
Abstract Item class. 
This class contains the bare minimum for an item:
	- Inventory support
	- Name/Description
	- Basic attributes
All items should derive from this class

See https://www.youtube.com/watch?v=FHYb63ppHmk&list=PLY1jY0hbmKxBvcEHa0k5Aw8_MKoB6jrRU&ab_channel=Arkeve
for inventory tutorial
*/

class Item : public Node2D {
	GODOT_CLASS(Item, Node2D)
public:
	void _init();
	static void _register_methods();
	virtual void _ready();

	// Still not sure if attributes are necessary. With inheritance and hyper-custom items, it might not be needed
	enum class Attributes {
		NONE = 0,
		// TODO: Add attributes
	};

	// For ItemDescription - see ItemDescription.hpp for more info
	// These are class members to make it easy to create the item description within the godot editor.
	String name, description, meta;

	Ref<ItemDescription> itemDescription;
	// Contains a list of all generic attributes for the item. See enum class Attributes
	unsigned int attributes;
	// Contains a texture for the item to be displayed in inventory/chests/etc
	// This can be the same as its dropped/held texture
	Ref<Texture> inventoryTexture;
	// Maximum size for a stack (idk how const works with godot, might have to change to a regular int)
	const int stackSize;

	// Wrapper that returns whether item has a specified attribute
	bool has_attribute(unsigned int attribute) const;

	// Generates item description. This will usually be overridden, unless it is a very basic item.
	virtual void gen_item_description();

	// Add virtual functions to handle input
	virtual void on_use() {}
};