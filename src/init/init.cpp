#include "../Common.hpp"
#include "../HUD/HUD.hpp"
#include "../Items/Item.hpp"
#include "../Items/Armor.hpp"
#include "../Items/Weapon.hpp"
#include "../Items/ItemSprite.hpp"
#include "../Text/ItemDescription.hpp"
#include "../Inventory/Inventory.hpp"
#include "../Inventory/InventorySlot.hpp"

extern "C" void GDN_EXPORT godot_gdnative_init(godot_gdnative_init_options* o)
{
    godot::Godot::gdnative_init(o);
}

extern "C" void GDN_EXPORT godot_gdnative_terminate(godot_gdnative_terminate_options* o)
{
    godot::Godot::gdnative_terminate(o);
}

extern "C" void GDN_EXPORT godot_nativescript_init(void* handle)
{
    godot::Godot::nativescript_init(handle);
    godot::register_class<HUD>();
    godot::register_class<ItemDescription>();
    godot::register_class<Item>();
    godot::register_class<ItemSprite>();
    godot::register_class<Armor>();
    godot::register_class<Weapon>();
    godot::register_class<InventorySlot>();
    godot::register_class<Inventory>();
}