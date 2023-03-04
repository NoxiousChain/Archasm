#pragma once

#include "Item.hpp"

class Weapon : public Item {
	GODOT_CLASS(Weapon, Item)

public:
	void _init();
	static void _register_methods();
	
	virtual void gen_item_description() override;


	double attack;
	double poisonDmg;	// Dmg/s
	double fireDmg;		// Dmg/s
};