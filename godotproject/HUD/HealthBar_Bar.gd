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
	# TODO: add functionality for different colors
	# Here is an implementation of a hue shift:

	# calculate difference between hues in radians
	var hue_diff_rad = abs(Color.red.h - col.h) * PI/2
	
	# start with heart sprite texture
	var img = heart_sprite.texture.get_data()
	# iterate through all pixels
	for x in range(img.get_width()):
		for y in range(img.get_height()):
			# get hsv value
			var hsv = img.get_pixelv(x, y).to_hsv()
			# adjust hue
			hsv.h += hue_diff_rad
			hsv.h = hsv.h % 1.0
			# set new hue value
			img.set_pixelv(x, y, hsv.to_rgb())
	# set texture
	var img_tex = ImageTexture.new()
	img_tex.create_from_image(img)
	heart_sprite.texture = img_tex
	
	# repeat for progress texture
	img = texture_progress.get_data()
	for x in range(img.get_width()):
		for y in range(img.get_height()):
			var hsv = img.get_pixelv(x, y).to_hsv()
			hsv.h += hue_diff_rad
			hsv.h = hsv.h % 1.0
			img.set_pixelv(x, y, hsv.to_rgb())
	img_tex.create_from_image(img)
	texture_progress = img_tex

	# Hue shift could be a bit slow. Might want to either just make the textures and switch between them, 
	# or I could try to do some shader work (not sure how easy that is to do with godot)
