#pragma once

#include "Item.hpp"

namespace godot {

class Armor : public Item {
	GODOT_CLASS(Armor, Item)
public:
	void _init();
	virtual static void _register_methods() override;

	double protection;
	double poison_protection;
	double fire_protection;
};

}