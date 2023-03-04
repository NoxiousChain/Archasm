#include "Armor.hpp"

void Armor::_init()
{
}

void Armor::_register_methods()
{
	register_property("protection", &Armor::protection, 0.0, GODOT_METHOD_RPC_MODE_DISABLED, GODOT_PROPERTY_USAGE_DEFAULT, GODOT_PROPERTY_HINT_RANGE, "0.0,1.0,.01,slider");
	register_property("poisonProtection", &Armor::poisonProtection, 0.0, GODOT_METHOD_RPC_MODE_DISABLED, GODOT_PROPERTY_USAGE_DEFAULT, GODOT_PROPERTY_HINT_RANGE, "0.0,1.0,.01,slider");
	register_property("fireProtection", &Armor::fireProtection, 0.0, GODOT_METHOD_RPC_MODE_DISABLED, GODOT_PROPERTY_USAGE_DEFAULT, GODOT_PROPERTY_HINT_RANGE, "0.0,1.0,.01,slider");
}
