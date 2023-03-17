#include "Inventory.hpp"

void Inventory::_init()
{
	Godot::print("Inventory::_init() called");
}

void Inventory::_register_methods()
{
	Godot::print("Inventory::_register_methods() called");

	register_method("_ready", &Inventory::_ready);
	register_method("_input", &Inventory::_input);
	register_method("_slot_gui_input", &Inventory::_slot_gui_input);
	register_method("save", &Inventory::save);
}

void Inventory::_ready()
{
	Godot::print("Inventory::_ready() called");

	// All nodes in the "Persist" group should have a save function (entire point of the group lol)
	add_to_group("Persist");
	bgTexture = cast_to<TextureRect>(get_node("TextureRect"));
	if (bgTexture == nullptr) {
		Godot::print("Inventory instance failed to get TextureRect node");
	}
	else {
		Godot::print("TextureRect node found");
	}

	gridContainer = cast_to<GridContainer>(get_node("GridContainer"));
	if (gridContainer == nullptr) {
		Godot::print("Inventory instance failed to get GridContainer node");
	}
	else {
		Godot::print("GridContainer node found");
	}

	// Iterates over all child nodes and connects their input to gui input signal
	for (int i = 0; i < gridContainer->get_child_count(); i++) {
		InventorySlot* child = cast_to<InventorySlot>(gridContainer->get_child(i));
		child->connect("gui_input", this, "_slot_gui_input", Array::make(child));
	}
}

void Inventory::_input(Ref<InputEvent> event)
{
	Input* _input = Input::get_singleton();
	if (_input == nullptr) return;
	Vector2 mousePos = get_viewport()->get_mouse_position();
	if (holdingItem != nullptr) {
		holdingItem->set_global_position(mousePos);
	}
	
}	

// Handles gui input to the inventory. This will likely be expanded more as more slots are added.
// Need to add support for stacking - this will require overloading == operator for items.
// Might also add support for shift-clicking and inventory sorting
// Note that all slot nodes follow the naming convention Slot[#] starting at 1 (e.g. get_node("Slot" + to_string(i+1)) would return slot at index i)
void Inventory::_slot_gui_input(Ref<InputEvent> event, InventorySlot* slot)
{
	Godot::print("inventory received input");
	const String eClass = event->get_class();
	if (eClass == "InputEventMouseButton") {
		Ref<InputEventMouseButton> mouse = cast_to<InputEventMouseButton>(event.ptr());
		// If left mouse button pressed
		if (mouse->get_button_index() == GlobalConstants::BUTTON_LEFT && mouse->is_pressed()) {
			// If holding an item
			if (holdingItem != nullptr) {
				// If full, swap held item with slot item
				if (slot->full()) {
					ItemSprite* temp = slot->take_from_slot();
					temp->set_global_position(mouse->get_global_position());
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
				Godot::print(holdingItem == nullptr ? "take_from_slot() returning nullptr" : "take_from_slot() functioning properly");
				// Update global position of held item sprite to follow mouse
				holdingItem->set_global_position(mouse->get_global_position());
			}
		}
		// allows me to generate an item when right clicking for testing purposes
		else if (mouse->get_button_index() == GlobalConstants::BUTTON_RIGHT && mouse->is_pressed()) {
			if (!slot->full()) {
				ResourceLoader* loader = ResourceLoader::get_singleton();
				Ref<PackedScene> res = loader->load("res://Inventory/Test_Item.tscn", "PackedScene");
				Node* scene = res->instance();
				slot->put_in_slot(cast_to<ItemSprite>(scene));
			}
		}
	}
}

Dictionary Inventory::save()
{
	// TODO: Serialization
	return Dictionary();
}