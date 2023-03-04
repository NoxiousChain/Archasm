#include "ItemDescription.hpp"

void ItemDescription::_init() 
{
	set_use_bbcode(true);
	set_scroll_active(false);
}

void ItemDescription::_register_methods()
{
	register_method("build", &ItemDescription::build);
}

void ItemDescription::build(const String& contents)
{
	// parses bbcode and handles formatting properly
	append_text(contents);
}