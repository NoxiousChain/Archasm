#include "Inventory.hpp"

void Inventory::_init()
{
	// All nodes in the "Persist" group should have a save function (entire point of the group lol)
	add_to_group("Persist"); 
	gridContainer = cast_to<GridContainer>(get_node("GridContainer"));
	if (gridContainer == nullptr) {
		Godot::print("Inventory instance failed to get GridContainer node");
	}
}

void Inventory::_register_methods()
{
	register_method("_ready", &Inventory::_ready);
	register_method("_input", &Inventory::_input);
	register_method("slot_gui_input", &Inventory::slot_gui_input);
	register_method("save", &Inventory::save);
}

void Inventory::_ready()
{
	// Iterates over all child nodes and connects their input to gui input signal
	for (int i = 0; i < gridContainer->get_child_count(); i++) {
		gridContainer->get_child(i)->connect("gui_input", this, "slot_gui_input");
	}

}

void Inventory::_input(Variant event)
{
	Ref<InputEventMouse> mouse = event;
	// Update global position of held item sprite to follow the mouse
	if (holdingItem != nullptr) {
		holdingItem->set_global_position(mouse->get_global_position());
	}
}

void Inventory::slot_gui_input(Variant event, InventorySlot* slot)
{
	Ref<InputEventMouseButton> mouseButton = event;
	// If left mouse button pressed
	if (mouseButton->get_button_index() == GlobalConstants::BUTTON_LEFT && mouseButton->is_pressed()) {
		// If holding an item
		if (holdingItem != nullptr) {
			// If full, swap held item with slot item
			if (slot->full()) {
				ItemSprite* temp = slot->take_from_slot();
				temp->set_global_position(mouseButton->get_global_position()));
				slot->put_in_slot(holdingItem);
				holdingItem = temp;
			}
			// Else just put held item into slot
			else {
				slot->put_in_slot(holdingItem);
				holdingItem = nullptr;
			}
		}
		// If not holding anything and slot full
		else if (slot->full()) {
			// Take from slot
			holdingItem = slot->take_from_slot();
			// Update global position of held item sprite to follow mouse
			holdingItem->set_global_position(mouseButton->get_global_position());
		}
	}
}

Dictionary Inventory::save()
{
	// TODO: Serialization
	return Dictionary();
}