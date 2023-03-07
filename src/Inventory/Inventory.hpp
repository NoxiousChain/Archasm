#pragma once

#include "../Common.hpp"
#include "InventorySlot.hpp"
#include <GridContainer.hpp>
#include <Node2D.hpp>
#include <Dictionary.hpp>

namespace godot 
{
	class Inventory : public Node2D
	{
		GODOT_CLASS(Inventory, Node2D)
	private:
		GridContainer* gridContainer;

	public:
		const unsigned int rows = 4, columns = 5;
		ItemSprite* holdingItem;

		void _init();
		static void _register_methods();
		void _ready();
		void _input();

		void slot_gui_input();

		void save();
	};

}