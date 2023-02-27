#include "HUD.hpp"

void HUD::_register_methods()
{
	register_method("_ready", &HUD::_ready);
	register_method("_process", &HUD::_process);
	register_method("pull", &HUD::pull);
	register_method("update_healthbar", &HUD::update_healthbar);
	register_method("run_healthbar_animation", &HUD::run_healthbar_animation);

	//register_property<HUD, Sprite*>("healthBar_Sprite", &HUD::healthBar_Sprite, nullptr);
	//register_property<HUD, TextureProgress*>("healthBar_Bar", &HUD::healthBar_Bar, nullptr);
}

void HUD::_init()
{
}

void HUD::_ready()
{
	Node* node = get_node(NodePath("HealthBar_Sprite"));
	if (node != nullptr) {
		healthBar_Sprite = Object::cast_to<Sprite>(node);
		if (healthBar_Sprite != nullptr) {
		}
		else {
			ERR_PRINT("get_node() failed on HealthBar_Sprite");
		}
	}
	node = get_node<TextureProgress>("HealthBar_Bar");
	if (node != nullptr) {
		healthBar_Bar = Object::cast_to<TextureProgress>(node);
		if (healthBar_Sprite != nullptr) {
			healthBar_Bar->set_value(1);
			healthBar_Bar->set_min(0);
			healthBar_Bar->set_max(1);
		}
		else {
			ERR_PRINT("get_node() failed on HealthBar_Bar");
		}
	}
}

void HUD::_process(float delta)
{
}

void HUD::pull(Vector2 from, float magnitude)
{
}

void HUD::update_healthbar(const float p_health, const float p_maxHealth, const int animation)
{
	// Clamp health value between 0-1 and set value based on that proportion
	healthBar_Bar->set_value(godot::Math::clamp(p_health, (real_t)0.f, 1.f));
	
	run_healthbar_animation(animation);
}

void HUD::run_healthbar_animation(const int animationType)
{
	// TODO: Add animations. These will probably just use particles and/or transforms.
}
