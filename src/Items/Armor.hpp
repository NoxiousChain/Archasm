#include "Item.hpp"

class Armor : public Item {
	GODOT_CLASS(Armor, Item)
public:
	void _init();
	static void _register_methods();

	double protection;
	double poisonProtection;
	double fireProtection;
};