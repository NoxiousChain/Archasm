extends TextureProgress

onready var heart_texture: Texture = load("res://resources/HUD/healthbar_heart_red.png")
onready var heart_sprite = Sprite.new()

# Called when the node enters the scene tree for the first time.
func _ready():
	heart_sprite.texture = heart_texture
	add_child(heart_sprite)
	# setting this manually cuz its a pain in the ass to do it relative to its parent
	# should automatically adjust for scale?
	heart_sprite.set_position(Vector2(64, 16)) 
	
func draw_heart(visible: bool = true):
	heart_sprite.visible = visible

func set_color(col: Color = Color.red):
	# Quick implementation of a hue shift. This is a pretty slow function,
	# because it makes an entire copy of each image, modifies each pixel individually,
	# and returns an entirely new texture. Should definitely be shader work, if possible.
	# However, this works, and fast enough to not affect gameplay much.
	# I could also rewrite this in c++ for better speed.
	
	# change hue for both heart and bar
	heart_sprite.texture = change_hue(heart_sprite.texture, col.h)
	texture_progress = change_hue(texture_progress, col.h)
	
	update()
	heart_sprite.update()

	# Hue shift is a bit slow. Might want to either just make the textures and switch between them, 
	# or I could try to do some shader work (not sure how easy that is to do with godot)

func change_hue(texture, target_hue):
	var img = texture.get_data()
	img.lock()
	
	# iterate thru all pixels
	for x in range(img.get_width()):
		for y in range(img.get_height()):
			# get pixel
			var pixel = img.get_pixel(x,y)
			# adjust hue
			pixel.h = target_hue
			# set new hue value
			img.set_pixel(x,y,pixel)
	# set texture
	var img_tex = ImageTexture.new()
	img_tex.create_from_image(img)
	return img_tex
