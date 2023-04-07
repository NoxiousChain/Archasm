#include "../Weapon.hpp"

namespace godot 
{
	class Dagger : public Weapon
	{
		GODOT_CLASS(Dagger, Weapon)
	protected:

	public:
		Dagger();
		~Dagger();

		void _init();
		static void _register_methods();
		virtual void _ready() override;

	protected:


	public:
		virtual void genItemDescription(MarginContainer* description_container, ItemDescription* item_description) override;
		virtual double calculateEffectDmg(float delta) override;
	};
}