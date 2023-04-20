#include "HealthBar_Bar.hpp"

void HealthBar_Bar::_register_methods() 
{
	register_method("_ready", &HealthBar_Bar::_ready);
	register_method("draw_heart", &HealthBar_Bar::drawHeart);
	register_method("set_color", &HealthBar_Bar::setColor);
	register_method("change_hue", &HealthBar_Bar::changeHue);
}

void HealthBar_Bar::_init()
{
}

void HealthBar_Bar::_ready()
{
	heartSprite = Sprite::_new();
	heartSprite->set_texture(ResourceLoader::get_singleton()->load("res://resources/HUD/healthbar_heart_red.png"));
	add_child(heartSprite);
	heartSprite->set_position(Vector2(64, 16));
}

void HealthBar_Bar::drawHeart(bool visible) 
{
	heartSprite->set_visible(visible);
}

void HealthBar_Bar::setColor(const Color& color)
{
	float targetHue = color.get_h();
	// Set textures with new textures from changeHue
	heartSprite->set_texture(changeHue(heartSprite->get_texture(), targetHue));
	this->set_progress_texture(changeHue(this->get_progress_texture(), targetHue));

	// Manually mark textures as needing to be updated
	this->update();
	heartSprite->update();
}

Ref<Texture> HealthBar_Bar::changeHue(Ref<Texture> texture, float targetHue)
{
	Ref<Image> img = texture->get_data();
	img->lock();

	// Iterate thru all pixels
	for (int x = 0; x < img->get_width(); x++) {
		for (int y = 0; y < img->get_height(); y++) {
			// Get pixel color
			Color pixel = img->get_pixel(x, y);
			// Set hsv using targetHue and previous s & v values
			pixel.set_hsv(targetHue, pixel.get_s(), pixel.get_v());
			img->set_pixel(x, y, pixel);
		}
	}
	img->unlock();
	
	Ref<ImageTexture> imgTex = ImageTexture::_new();
	imgTex->create_from_image(img);
	return imgTex;
}