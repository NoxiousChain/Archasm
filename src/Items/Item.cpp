#include "Item.hpp"

Item::~Item()
{
	description_container->queue_free();
	description_container = nullptr;
}

void Item::_init()
{
	description_container = nullptr;
	item_description = nullptr;
	attributes = 0;
	texture = Ref<Texture>();
	stack_size = 1;
	quantity = 1;
}

void Item::register_methods()
{
	register_method("_ready", &Item::_ready);
	register_method("has_attribute", &Item::has_attribute);
	register_method("show_description", &Item::show_description);
	register_method("gen_item_description", &Item::gen_item_description);

	register_property<Item, String>("name", &Item::name, "");
	register_property<Item, String>("description", &Item::description, "", GODOT_METHOD_RPC_MODE_DISABLED, GODOT_PROPERTY_USAGE_DEFAULT, GODOT_PROPERTY_HINT_MULTILINE_TEXT, "General description (e.g. an axe - \"A simple tool used for chopping.\")");
	register_property<Item, String>("meta", &Item::meta, "", GODOT_METHOD_RPC_MODE_DISABLED, GODOT_PROPERTY_USAGE_DEFAULT, GODOT_PROPERTY_HINT_MULTILINE_TEXT, "Meta text. This is displayed at the bottom of the description. Italicize by convention.");
	register_property<Item, Ref<Texture>>("texture", &Item::texture, Ref<Texture>());
	register_property<Item, unsigned int>("attributes", &Item::attributes, (unsigned int)Attributes::NONE,
		GODOT_METHOD_RPC_MODE_DISABLED, GODOT_PROPERTY_USAGE_DEFAULT, GODOT_PROPERTY_HINT_FLAGS,
		"NONE, FLAG1, FLAG2, ..."); // TODO: Update when attributes added
	register_property<Item, int>("stack_size", &Item::stack_size, 1);
	register_property<Item, int>("quantity", &Item::quantity, 1);
}

void Item::_register_methods()
{
	register_methods();
}

void Item::_ready()
{
	item_description = ItemDescription::_new();
	gen_item_description();
	description_container = MarginContainer::_new();
	description_container->set_owner(this);
	description_container->add_child(item_description);
	item_description->set_owner(description_container);
}

bool Item::has_attribute(unsigned int attribute) const
{
	return attributes & attribute;
}

void Item::show_description(bool visible)
{
	if (item_description != nullptr) {
		// Draw background, etc
		item_description->set_visible(visible);
	}
	else ERR_PRINT("Item description does not exist!");
}

void Item::gen_item_description()
{
	description_container->set_custom_minimum_size(Vector2(150, 250));
	description_container->set_visible(false);
	description_container->add_constant_override("margin_top", 10);
	description_container->add_constant_override("margin_bottom", 10);
	description_container->add_constant_override("margin_left", 10);
	description_container->add_constant_override("margin_right", 10);
	item_description->build(
		name + "\n\n" + description + (meta.empty() ? "" : "\n\n" + meta)
	);
}

Ref<Texture> Item::get_texture()
{
	return texture;
}