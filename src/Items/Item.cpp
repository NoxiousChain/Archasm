#include "Item.hpp"

Item::Item()
{
}

Item::~Item()
{
}

void Item::_init()
{
	Godot::print("Item::_init() called");

	attributes = 0;
	texture = Ref<Texture>();
	stackSize = 1;
	quantity = 1;
}

void Item::_register_methods()
{
	Godot::print("Item::_register_methods() called");

	register_method("_ready", &Item::_ready);
	register_method("genDescriptionContainer", &Item::genDescriptionContainer);
	register_method("genItemDescription", &Item::genItemDescription);
	register_method("getTexture", &Item::getTexture);
	register_method("getName", &Item::getName);
	register_method("getStackSize", &Item::getStackSize);
	register_method("getQuantity", &Item::getQuantity);

	register_property<Item, String>("name", &Item::name, "");
	register_property<Item, String>("description", &Item::description, "", GODOT_METHOD_RPC_MODE_DISABLED, GODOT_PROPERTY_USAGE_DEFAULT, GODOT_PROPERTY_HINT_MULTILINE_TEXT, "General description (e.g. an axe - \"A simple tool used for chopping.\")");
	register_property<Item, String>("meta", &Item::meta, "", GODOT_METHOD_RPC_MODE_DISABLED, GODOT_PROPERTY_USAGE_DEFAULT, GODOT_PROPERTY_HINT_MULTILINE_TEXT, "Meta text. This is displayed at the bottom of the description. Italicize by convention.");
	register_property<Item, Ref<Texture>>("texture", &Item::texture, Ref<Texture>());
	register_property<Item, int>("stackSize", &Item::stackSize, 1);
	register_property<Item, int>("quantity", &Item::quantity, 1);
}

void Item::_ready()
{
	Godot::print("Item::_ready() called");
}

Ref<Texture> Item::getTexture() const
{
	return texture;
}

String Item::getName() const
{
	return name;
}

int Item::getStackSize() const
{
	return stackSize;
}

int Item::getQuantity() const
{
	return quantity;
}

void Item::genDescriptionContainer(MarginContainer* description_container)
{
	if (description_container == nullptr) return;
	description_container->set_custom_minimum_size(Vector2(150, 250));
	description_container->set_visible(false);
	description_container->add_constant_override("margin_top", 10);
	description_container->add_constant_override("margin_bottom", 10);
	description_container->add_constant_override("margin_left", 10);
	description_container->add_constant_override("margin_right", 10);
}

void Item::genItemDescription(MarginContainer* description_container, ItemDescription* item_description)
{
	genDescriptionContainer(description_container);
	item_description->build(
		name + "\n\n" + description + (meta.empty() ? "" : "\n\n" + meta)
	);
}

void Item::on_use()
{
}