#include "Weapon.hpp"

void Weapon::_init()
{
	effects = 0;
	attack = 0.0;
	poisonDmg = 0.0;
	fireDmg = 0.0;
}

void Weapon::_register_methods()
{
	Godot::print("Weapon::_register_methods() called");

	register_property<Weapon, unsigned int>("effects", &Weapon::effects, 0U, GODOT_METHOD_RPC_MODE_DISABLED, GODOT_PROPERTY_USAGE_DEFAULT, GODOT_PROPERTY_HINT_FLAGS,
		"LIGHT, HEAVY, BLEED, QUICK, STUN, RANGE, DEFENSE");
	register_property<Weapon, double>("attack", &Weapon::attack, 0.0);
	register_property<Weapon, double>("poisonDmg", &Weapon::poisonDmg, 0.0);
	register_property<Weapon, double>("fireDmg", &Weapon::fireDmg, 0.0);

	register_method("genItemDescription", &Weapon::genItemDescription);
}

void Weapon::genItemDescription(MarginContainer* description_container, ItemDescription* item_description)
{
	genDescriptionContainer(description_container);
}
