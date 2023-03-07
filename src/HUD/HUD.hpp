#pragma once
#include "../Common.hpp"
#include <CanvasLayer.hpp>
#include <unordered_map>
#include <core/Ref.hpp>
#include <Sprite.hpp>
#include <TextureProgress.hpp>
#include <Node.hpp>
#include <Math.hpp>
#include <Texture.hpp>
#include <Input.hpp>

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
	Sprite* healthbar_sprite;
	TextureProgress* healthbar_bar;

	/// @brief Runs animation on healthbar when player health status changes
	/// @param animation_type Cast enum to int e.g. (int)HealthBarAnimationType::TYPE
	void run_healthbar_animation(const int animation_type);
public:
	// Registers all of the methods that need to be accessible
	static void _register_methods();
	// Called when the object is initialized into memory. Replaces constructor
	void _init();
	// Called when the node has entered the scene tree. Closer to a normal init() function
	void _ready();
	void _process(float delta);

	void pull(Vector2 from, float magnitude);
	
	/// @brief Updates health bar graphics based on health percentage
	/// @param p_health Player health
	/// @param p_maxHealth Player maximum health
	/// @param animation Animation type to play. This is dependent on the current player state (taking damage, healing, etc.). Use (int)HealthBarAnimationType::TYPE
	void update_healthbar(const float p_health, const float p_max_health, const int animation);
};