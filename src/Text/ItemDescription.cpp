#include "ItemDescription.hpp"

void ItemDescription::_init() 
{
	Godot::print("ItemDescription::_init() called");
	set_use_bbcode(true);
	set_scroll_active(false);
	set_fit_content_height(true);
}

void ItemDescription::_register_methods()
{
	Godot::print("ItemDescription::_register_methods() called");
	register_method("build", &ItemDescription::build);
}

void ItemDescription::build(const String& contents)
{
	// parses bbcode and handles formatting properly
	append_bbcode(contents);
}