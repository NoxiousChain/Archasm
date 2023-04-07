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
			LIGHT = 1,			// +10% movement speed
			HEAVY = 2,			// -5% accuracy, -10% movement speed
			BLEED = 4,			// 20% chance to "bleed" enemy, causing -10% health/s for 3s
			QUICK = 8,			// 35% chance to double attack
			STUN = 16,			// 20% chance to immobilize enemy for 1.5s
			DEFENSE = 32,		// 15% chance to block enemy attack
		};

		unsigned int effects;

		double attack;
		double poisonDmg;	// Dmg/s
		double fireDmg;		// Dmg/s

	public:
		virtual double calculateEffectDmg(float delta);
		 
	};

}