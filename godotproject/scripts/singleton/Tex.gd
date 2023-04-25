extends Node


enum {
	HAND = -1,
	DIRT = 0,
	DIRT_GRASS = 1,
	GRAVEL = 2,
	VINE = 3,
	GRASS = 4,
	ROSE = 5,
	PAQUERETTE = 6,
	TULIPE = 7,
	TORCH = 8,
	VINE_CORD = 9,
	TRUNK_LOW = 10,
	TRUNK = 11,
	TRUNK_TOP = 12,
	STONE = 13,
	
	PURPLEA = 14,
	
	DIRT_1 = 21,
	DIRT_2 = 22,
	DIRT_3 = 23,
	DIRT_GRASS_1 = 24,
	DIRT_GRASS_2 = 25, 
	DIRT_GRASS_3 = 26,
	GRAVEL_1 = 27,
	STONE_1 = 28,
	
}

var tiles_tex = {
	DIRT: [preload("res://resources/tiles/plains/dirt.png"), Vector2(10,10), DIRT],
	DIRT_GRASS: [preload("res://resources/tiles/plains/grass.png"), Vector2(10,10), DIRT_GRASS],
	GRAVEL: [preload("res://resources/tiles/plains/gravel.png"), Vector2(10,10), GRAVEL],
	STONE: [preload("res://resources/tiles/plains/stone.png"), Vector2(10,10), STONE],
	TRUNK: [preload("res://resources/tiles/plains/obstacles/treeTrunk.png"), Vector2(6,10), TRUNK],
	TRUNK_LOW: [preload("res://resources/tiles/plains/obstacles/treeTrunk.png"), Vector2(6,10), TRUNK],
	TRUNK_TOP = null,
	
	GRASS: [preload("res://resources/tiles/plains/obstacles/grass.png"), Vector2(4,4), GRASS],
	ROSE: [preload("res://resources/tiles/plains/obstacles/rose.png"), Vector2(4,4), ROSE],
	PAQUERETTE: [preload("res://resources/tiles/plains/obstacles/paquerette.png"), Vector2(4,4), PAQUERETTE],
	TULIPE: [preload("res://resources/tiles/plains/obstacles/tulipe.png"), Vector2(4,4), TULIPE],
	
}

var similar_list = {
	DIRT: [DIRT, DIRT_1, DIRT_2, DIRT_3],
	DIRT_GRASS: [DIRT_GRASS, DIRT_GRASS_1, DIRT_GRASS_2, DIRT_GRASS_3],
	GRAVEL: [GRAVEL, GRAVEL_1],
	STONE: [STONE, STONE_1],
}

var share_identity = {
	DIRT_1: DIRT,
	DIRT_2: DIRT,
	DIRT_3: DIRT,
	DIRT_GRASS_1: DIRT,
	DIRT_GRASS_2: DIRT, 
	DIRT_GRASS_3: DIRT, 
	GRAVEL_1: GRAVEL,
	STONE_1: STONE,
}

func res_sims(cell: int):
	var resolve = cell
	if share_identity.has(cell):
		resolve = share_identity[cell]
	return resolve
	
var cell_top = {
	DIRT: DIRT_GRASS,
}

var cell_down = {
	DIRT_GRASS: DIRT,
	DIRT_GRASS_1: DIRT_1,
	DIRT_GRASS_2: DIRT_2,
	DIRT_GRASS_3: DIRT_3,
}

var tiles_name = {
	DIRT: ["Dirt", ""],
	GRAVEL: ["Gravel", ""],
	STONE: ["Stone", ""],
	TRUNK: ["Wood", ""],
	
	GRASS: ["Grass", ""], ROSE: ["Rose", ""], PAQUERETTE: ["Daisy", ""], TULIPE: ["Tulip", ""],
	PURPLEA: ["Purplea", ""],
	
}

var block_tiles = [
	DIRT, DIRT_GRASS, GRAVEL, STONE,
]

var weapon_object = [
	HAND,
]

var hand_object = [
	HAND, TORCH,
]

var unstickable_tiles = [
	TORCH, VINE_CORD, GRASS, ROSE, PAQUERETTE, TULIPE, PURPLEA,
	TRUNK_TOP, TRUNK, TRUNK_LOW,
]

var flower_tiles = [
	GRASS, ROSE, PAQUERETTE, TULIPE, PURPLEA,
]

var trunk_tiles = [
	TRUNK_TOP, TRUNK, TRUNK_LOW
]

var crafting = {
	#could be used for future use
}

var weapon = {
	HAND: {
		"ability": {},
		"damage": 20, "lvl": 0, "rate": 0.4, "range": 96, "img": null,
	}
}
var block_stat = {
	DIRT: {"hp": 55, "lvl": 0},
	DIRT_GRASS: {"hp": 55, "lvl": 0},
	GRAVEL: {"hp": 40, "lvl": 0},
	STONE: {"hp": 100, "lvl": 1},
	TRUNK_LOW: {"hp": 140, "lvl": 0},
	TRUNK: {"hp": 140, "lvl": 0},
	TRUNK_TOP: {"hp": 0, "lvl": -1},
	
	GRASS: {"hp": 0, "lvl": -1},
	ROSE: {"hp": 0, "lvl": -1},
	PAQUERETTE: {"hp": 0, "lvl": -1},
	TULIPE: {"hp": 0, "lvl": -1},
	PURPLEA: {"hp": 0, "lvl": -1},
}

var hand = {
	HAND: {
		"ability": {},
		"img": null, "anim": {"h": 1, "v": 1}},
#	TORCHE: {
#		"ability": {"light_color": Color(1,0.8,0.5,0.1), "light_range": 180, "min_light_range": 96},
#		"img": preload("res://image/tiles/torche.png"), "anim": {"h": 3, "v": 2}},
}

var fonts = {
	1 : preload("res://resources/fonts/recurso/RecursoSans-Medium.ttf")
}

func _ready():
	trunk_tiles = add_similar_tiles(trunk_tiles)
	
func add_similar_tiles(list):
	for x in list:
		if similar_list.has(x):
			for t in similar_list[x]:
				if !list.has(t): list.append(t)
	return list
