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

		enum class Effects {
			LIGHT = 1,
			HEAVY = 2,
			BLEED = 4,
			QUICK = 8,
			STUN = 16,
			RANGE = 32,
			DEFENSE = 64
		};

		unsigned int effects;

		double attack;
		double poisonDmg;	// Dmg/s
		double fireDmg;	// Dmg/s

	};

}