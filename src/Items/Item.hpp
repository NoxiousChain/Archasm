#pragma once

#include "../Common.hpp"
#include <Resource.hpp>
#include <Ref.hpp>
#include <Texture.hpp>
#include "../Text/ItemDescription.hpp"
#include <MarginContainer.hpp>

/*
Abstract Item class. 
This class contains the bare minimum for an item:
	- Inventory support
	- Name/Description
	- Basic attributes
	- Textures
	- Maximum stack size
	- Total 
All items should derive from this class
*/
namespace godot {

class Item : public Resource {
	GODOT_CLASS(Item, Resource)
public:
	~Item();

	void _init();
	// Because _register_methods() needs to be overridden for inherited classes, this contains all of the methods for the Item
	// class so that inherited classes can just call register_methods() instead of rewriting the same code
	// This allows for easy item creation through the godot editor
	virtual static void register_methods();
	virtual static void _register_methods();
	virtual void _ready();

	// Still not sure if attributes are necessary. With inheritance and hyper-custom items, it might not be needed
	enum class Attributes {
		NONE = 0,
		// TODO: Add attributes
	};

	// For ItemDescription - see ItemDescription.hpp for more info
	// These are class members to make it easy to create the item description within the godot editor.
	String name, description, meta;

	MarginContainer* description_container;
	ItemDescription* item_description;
	// Contains a list of all generic attributes for the item. See enum class Attributes
	unsigned int attributes;
	// Texture for the item
	Ref<Texture> texture;
	// Maximum size for a stack
	int stack_size;
	// Quantity of existing item
	int quantity;

	// Wrapper that returns whether item has a specified attribute
	bool has_attribute(unsigned int attribute) const;
	void show_description(bool visible = true);

	// Getters/Setters
	// These are for thread safety
	Ref<Texture> get_texture() const;

	// Generates item description. This will usually be overridden, unless it is a very basic item.
	virtual void gen_item_description();

	// Add virtual functions to handle input
	virtual void on_use() {}
};

}