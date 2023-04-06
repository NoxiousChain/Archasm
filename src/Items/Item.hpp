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
All items should derive from this class.

Important to note: Because this derives from a resource, it is saved to disk
When making a new item, create a new resource and link an item-derived script to it
Then any new instances of this item will be thatResource.instance(), allowing custom
modifications without changing the main components of the resource
*/

namespace godot {

	class Item : public Resource {
		GODOT_CLASS(Item, Resource)
	public:
		Item();
		~Item();

		void _init();
		static void _register_methods();
		virtual void _ready();

	protected:
		// For ItemDescription - see ItemDescription.hpp for more info
		// These are class members to make it easy to create the item description within the godot editor.
		String name, description, meta;

		// Contains a list of all generic attributes for the item. See enum class Attributes
		unsigned int attributes;
		// Texture for the item
		Ref<Texture> texture;
		// Maximum size for a stack
		int stackSize;
		// Quantity of existing item
		int quantity;

	public:

		// Getters/Setters
		Ref<Texture> getTexture() const;
		String getName() const;
		int getStackSize() const;
		int getQuantity() const;

		// Generates common values for description_container. All overrides of genItemDescription should
		// call this function.
		void genDescriptionContainer(MarginContainer* description_container);
		// Generates item description. This will usually be overridden, unless it is a very basic item. 
		// This function should only be called by an ItemSprite instance.
		virtual void genItemDescription(MarginContainer* description_container, ItemDescription* item_description);

		// TODO: Add virtual functions to handle input
		virtual void on_use();
	};

}