#pragma once

#include "Item.hpp"

namespace godot {

class Weapon : public Item {
	GODOT_CLASS(Weapon, Item)

public:
	void _init();
	static void _register_methods();
	
	virtual void gen_item_description() override;

	enum class Effects {
		LIGHT = 2,
		HEAVY = 4,
		BLEED = 8,
		QUICK = 16,
		STUN = 32,
		RANGE = 64,
		DEFENSE = 128,
		AUTISM = 256
	};

	double attack;
	double poison_dmg;	// Dmg/s
	double fire_dmg;	// Dmg/s
	
};

}