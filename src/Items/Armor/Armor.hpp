#pragma once

#include "Item.hpp"

namespace godot {

	class Armor : public Item {
		GODOT_CLASS(Armor, Item)
	public:
		void _init();
		static void _register_methods();
		virtual void genItemDescription(MarginContainer* description_container, ItemDescription* item_description) override;


		double protection;
		double poison_protection;
		double fire_protection;
	};

}