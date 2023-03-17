#pragma once

#include "../Common.hpp"
#include <GlobalConstants.hpp>
#include "InventorySlot.hpp"
#include <GridContainer.hpp>
#include <Node2D.hpp>
#include <Dictionary.hpp>
#include <InputEvent.hpp>
#include <InputEventMouseButton.hpp>
#include <PackedScene.hpp>
#include <SceneTree.hpp>
#include <Viewport.hpp>
#include <string>

namespace godot {

	class Inventory : public Node2D
	{
		GODOT_CLASS(Inventory, Node2D)
	private:
		GridContainer* gridContainer = nullptr;
		ItemSprite* holdingItem = nullptr;
		TextureRect* bgTexture = nullptr;
		// Add a second grid container + subnodes for hotbar
	public:
		void _init();
		static void _register_methods();
		void _ready();
		void _input(Ref<InputEvent> event);

		void _slot_gui_input(Ref<InputEvent> event, InventorySlot* slot);

		Dictionary save();
	};

}