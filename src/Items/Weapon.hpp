#pragma once

#include "Item.hpp"

namespace godot {

	class Weapon : public Item {
		GODOT_CLASS(Weapon, Item)

	public:
		void _init();
		static void _register_methods();

		virtual void genItemDescription(MarginContainer* description_container, ItemDescription* item_description) override;

	protected:
		// For use with item description, coloring, effects, etc. 
		enum class Rarity {
			COMMON = 1,
			UNUSUAL = 2,
			EXCEPTIONAL = 4,
			ANCIENT = 8,
			MYTHIC = 16,
			LEGENDARY = 32
		};

		// Standard effects that most weapons will have
		// Any custom effects for higher tier weapons should be implemented in their derived classes and do not need an enum
		enum class Effects {
			LIGHT = 1,
			HEAVY = 2,
			BLEED = 4,
			QUICK = 8,
			STUN = 16,
			DEFENSE = 32
		};

		unsigned int rarity, effects;

		double attack;

	};

public:
	virtual double getDamage(); // Returns the damage on an initial hit. This is based solely on attack damage and any effects
	virtual void onAttack();
	virtual void onAltAttack();
}