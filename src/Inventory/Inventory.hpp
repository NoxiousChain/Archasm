#pragma once

#include "../Common.hpp"
#include <GlobalConstants.hpp>
#include "InventorySlot.hpp"
#include <GridContainer.hpp>
#include <Node2D.hpp>
#include <Dictionary.hpp>
#include <InputEvent.hpp>
#include <InputEventMouseButton.hpp>
#include <button>

namespace godot 
{
class Inventory : public Node2D
{
	GODOT_CLASS(Inventory, Node2D)
private:
	GridContainer* gridContainer;
	ItemSprite* holdingItem;
public:
	void _init();
	static void _register_methods();
	void _ready();
	void _input();

	void slot_gui_input(InputEvent event, InventorySlot* slot);

	Dictionary save();
};

}