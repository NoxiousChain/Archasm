#pragma once

#include "Common.hpp"
#include <MarginContainer.hpp>

class HUD : public MarginContainer {
	GODOT_CLASS(HUD, MarginContainer);

public:
	static void _register_methods();
	void _init();

};