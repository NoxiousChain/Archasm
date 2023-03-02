#include "Item.hpp"

class Armor : public Item {
	GODOT_CLASS(Armor, Item)
public:
	void _init();
	static void _register_methods();

	struct ProtModifiers {
		double protection;
		// Add more modifiers e.g. poison
	} protModifiers;

};