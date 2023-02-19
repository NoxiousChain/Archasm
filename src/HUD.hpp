#pragma once

#include "Common.hpp"
#include <CanvasLayer.hpp>
#include <unordered_map>
#include <core/Ref.hpp>
#include <Sprite.hpp>
#include <TextureProgress.hpp>
#include <Node.hpp>
#include <Math.hpp>
#include <Texture.hpp>

class HUD : public CanvasLayer {
	GODOT_CLASS(HUD, CanvasLayer)

public:
	// Godot doesn't have proper enum support, so any use of this enum must be converted to int
	// Functions cannot have an enum as a parameter
	enum class HealthBarAnimationType {
		DAMAGE,		// Player takes damage - healthbar shakes and bar decay shown
		HEAL		// Player is healed - bubbly animation plays
		// POISON	// example of another possible type
	};

private:
	Sprite* healthBar_Sprite;
	TextureProgress* healthBar_Bar;

	/// @brief Runs animation on healthbar when player health status changes
	/// @param animationType Cast enum to int e.g. (int)HealthBarAnimationType::TYPE
	void run_healthbar_animation(const int animationType);
public:
	static void _register_methods();
	void _init();
	void _ready();
	void _process(float delta);

	void pull(Vector2 from, float magnitude);
	
	/// @brief Updates health bar graphics based on health percentage
	/// @param p_health Player health
	/// @param p_maxHealth Player maximum health
	/// @param animation Animation type to play. This is dependent on the current player state (taking damage, healing, etc.). Use (int)HealthBarAnimationType::TYPE
	void update_healthbar(const float p_health, const float p_maxHealth, const int animation);
};