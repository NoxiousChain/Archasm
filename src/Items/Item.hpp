#pragma once

#include "../Common.hpp"
#include <Node2D.hpp>
#include <Reference.hpp>
#include <Sprite.hpp>
#include <CollisionShape2D.hpp>
#include <Texture.hpp>
#include <Ref.hpp>

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

	enum class Attributes {
		// TODO: Add attributes
	};

protected:
	String description;
	unsigned int attributes;

public:
	String get_description() const;
	bool has_attribute(unsigned int attribute) const;
};

class Weapon : public Item {
	GODOT_CLASS(Weapon, Item)

public:
	void _init();
	static void _register_methods();

public:
};

class Armor : public Item {
	GODOT_CLASS(Armor, Item)
public:
	void _init();
	static void _register_methods();

protected:
};