#pragma once
#include "../Common.hpp"
#include <TextureProgress.hpp>
#include <Texture.hpp>
#include <ImageTexture.hpp>
#include <Sprite.hpp>
#include <Color.hpp>
#include <ResourceLoader.hpp>
#include <Ref.hpp>
#include <Image.hpp>

class HealthBar_Bar : public TextureProgress {
	GODOT_CLASS(HealthBar_Bar, TextureProgress)

private:
	Sprite* heartSprite;

public:
	static void _register_methods();
	void _init();
	void _ready();
	
	void drawHeart(bool visible = true);
	void setColor(const Color& color = Color(1, 0, 0));

private:
	Ref<Texture> changeHue(Ref<Texture> texture, float targetHue);
};