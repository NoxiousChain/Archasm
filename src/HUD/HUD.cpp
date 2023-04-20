#include "HUD.hpp"

void HUD::_register_methods()
{
	register_method("_ready", &HUD::_ready);
	register_method("_process", &HUD::_process);
	register_method("pull", &HUD::pull);
	register_method("update_healthbar", &HUD::update_healthbar);
	register_method("run_healthbar_animation", &HUD::run_healthbar_animation);
}

void HUD::_init()
{
}

void HUD::_ready()
{
	Node* node = get_node("HealthBar_Sprite");
	if (node != nullptr) {
		healthbar_sprite = Object::cast_to<Sprite>(node);
		if (healthbar_sprite != nullptr) {
		}
		else {
			ERR_PRINT("get_node() failed on HealthBar_Sprite");
		}
	}
	node = get_node("HealthBar_Bar");
	//node = get_node(NodePath("HealthBar_Bar"));
	if (node != nullptr) {
		healthbar_bar = Object::cast_to<HealthBar_Bar>(node);
		if (healthbar_bar != nullptr) {
			healthbar_bar->set_value(1);
			healthbar_bar->set_min(0);
			healthbar_bar->set_max(1);
			healthbar_bar->drawHeart();
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

void HUD::update_healthbar(const float p_health, const float p_max_health, const int animation)
{
	float health = p_health / p_max_health;
	if (health <= 0) {
		healthbar_bar->drawHeart(false);
		//healthbar_bar->call("draw_heart", Array::make(false));
	}
	// healthbar value must be between 0-1
	healthbar_bar->set_value(p_health / p_max_health);
	run_healthbar_animation(animation);
}

void HUD::run_healthbar_animation(const int animation_type)
{
	// TODO: Add animations. These will probably just use particles and/or transforms.
}
