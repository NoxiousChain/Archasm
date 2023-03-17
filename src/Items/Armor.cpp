#include "Armor.hpp"

void Armor::_init()
{
}

void Armor::_register_methods()
{
	register_property("protection", &Armor::protection, 0.0, GODOT_METHOD_RPC_MODE_DISABLED, GODOT_PROPERTY_USAGE_DEFAULT, GODOT_PROPERTY_HINT_RANGE, "0.0,1.0,.01,slider");
	register_property("poison_protection", &Armor::poison_protection, 0.0, GODOT_METHOD_RPC_MODE_DISABLED, GODOT_PROPERTY_USAGE_DEFAULT, GODOT_PROPERTY_HINT_RANGE, "0.0,1.0,.01,slider");
	register_property("fire_protection", &Armor::fire_protection, 0.0, GODOT_METHOD_RPC_MODE_DISABLED, GODOT_PROPERTY_USAGE_DEFAULT, GODOT_PROPERTY_HINT_RANGE, "0.0,1.0,.01,slider");
}

void Armor::genItemDescription(MarginContainer* description_container, ItemDescription* item_description)
{
	genDescriptionContainer(description_container);
}
