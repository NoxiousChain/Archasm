#include "Item.hpp"

void Item::_init()
{
	
}

void Item::_register_methods()
{
	register_method("has_attribute", &Item::has_attribute);
	register_method("gen_item_description", &Item::gen_item_description);

	register_property<Item, String>("Name", &Item::name, "");
	register_property<Item, String>("Description", &Item::description, "", GODOT_METHOD_RPC_MODE_DISABLED, GODOT_PROPERTY_USAGE_DEFAULT, GODOT_PROPERTY_HINT_MULTILINE_TEXT, "General description (e.g. an axe - \"A simple tool used for chopping.\")");
	register_property<Item, String>("Meta", &Item::meta, "", GODOT_METHOD_RPC_MODE_DISABLED, GODOT_PROPERTY_USAGE_DEFAULT, GODOT_PROPERTY_HINT_MULTILINE_TEXT, "Meta text. This is displayed at the bottom of the description. Italicize by convention.");
	register_property<Item, Ref<ItemDescription>>("Item Description", &Item::itemDescription, Ref<ItemDescription>());
	register_property<Item, Ref<Texture>>("Inventory Texture", &Item::inventoryTexture, Ref<Texture>());
	register_property<Item, unsigned int>("Attributes", &Item::attributes, (unsigned int)Attributes::NONE,
		GODOT_METHOD_RPC_MODE_DISABLED, GODOT_PROPERTY_USAGE_DEFAULT, GODOT_PROPERTY_HINT_FLAGS,
		"NONE, FLAG1, FLAG2, ..."); // TODO: Update when attributes added
	register_property<Item, const int>("Stack size", &Item::stackSize, 1);

}

void Item::_ready()
{
	description = ItemDescription::_new();
	gen_item_description();
}

bool Item::has_attribute(unsigned int attribute) const
{
	return attributes & attribute;
}

void Item::gen_item_description()
{
	itemDescription->build(
		name + "\n\n" + description + (meta.empty() ? "" : "\n\n" + meta)
	);
}
