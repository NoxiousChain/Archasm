#pragma once

#include "../Common.hpp"
#include <Panel.hpp>
#include <ResourceLoader.hpp>
#include <StyleBoxTexture.hpp>
#include <Texture.hpp>
#include "../Items/ItemSprite.hpp"
#include <Ref.hpp>
#include <Input.hpp>

namespace godot {

	class InventorySlot : public Panel {
		GODOT_CLASS(InventorySlot, Panel)

	private:
		ItemSprite* itemSprite = nullptr;
		Ref<StyleBoxTexture> style;
		Ref<Texture> defaultStyleTexture, emptyStyleTexture;

	public:
		InventorySlot();
		~InventorySlot();
		void _init();
		static void _register_methods();

		void _ready();
		void refreshStyle();

		// Getters/setters
		Ref<Texture> _getDefaultStyleTexture() const;
		Ref<Texture> _getEmptyStyleTexture() const;
		void _setDefaultStyleTexture(Ref<Texture> other);
		void _setEmptyStyleTexture(Ref<Texture> other);

		// returns whether the slot is already occupied by an item
		bool full();
		// returns a pointer to the item (nullptr if dne)
		ItemSprite* getItem();
		ItemSprite* take_from_slot();
		void put_in_slot(ItemSprite* item);
	};

}