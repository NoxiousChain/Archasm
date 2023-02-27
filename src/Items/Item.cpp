#include "Item.hpp"

void Item::_init()
{
	
}

void Item::_register_methods()
{
	register_method("get_description", &Item::get_description);
	register_method("has_attribute", &Item::has_attribute);
}

String Item::get_description() const
{
	return description;
}

bool Item::has_attribute(unsigned int attribute) const
{
	return attributes & attribute;
}

void Weapon::_init()
{
}

void Weapon::_register_methods()
{
}

void Armor::_init()
{
}

void Armor::_register_methods()
{
}
