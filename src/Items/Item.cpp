#include "Item.hpp"

void Item::_init()
{
	
}

void Item::_register_methods()
{
	register_method("has_attribute", &Item::has_attribute);

	register_property<Item, String>("Description", &Item::description, String());
	register_property<Item, Ref<Texture>>("Inventory Texture", &Item::inventoryTexture, Ref<Texture>());
	register_property<Item, unsigned int>("Attributes", &Item::attributes, (unsigned int)Attributes::NONE,
		GODOT_METHOD_RPC_MODE_DISABLED, GODOT_PROPERTY_USAGE_DEFAULT, GODOT_PROPERTY_HINT_FLAGS,
		"NONE, FLAG1, FLAG2, ..."); // TODO: Update when attributes added
	register_property<Item, const int>("Stack size", &Item::stackSize, 1);

}

bool Item::has_attribute(unsigned int attribute) const
{
	return attributes & attribute;
}