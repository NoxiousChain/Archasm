#pragma once

#include "Item.hpp"

class Weapon : public Item {
	GODOT_CLASS(Weapon, Item)

public:
	void _init();
	static void _register_methods();

public:

};