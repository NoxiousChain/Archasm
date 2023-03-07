#pragma once

#include "Item.hpp"

namespace godot {

class Weapon : public Item {
	GODOT_CLASS(Weapon, Item)

public:
	void _init();
	static void _register_methods();
	
	virtual void gen_item_description() override;


	double attack;
	double poison_dmg;	// Dmg/s
	double fire_dmg;	// Dmg/s
};

}