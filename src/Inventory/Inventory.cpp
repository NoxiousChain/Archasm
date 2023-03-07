#include "Inventory.hpp"

void Inventory::_init()
{
	// All nodes in the "Persist" group should have a save function (entire point of the group lol)
	add_to_group("Persist"); 
}

void Inventory::_register_methods()
{

}

void Inventory::_ready()
{

}

void Inventory::_input()
{

}

void Inventory::slot_gui_input()
{

}

void Inventory::save();