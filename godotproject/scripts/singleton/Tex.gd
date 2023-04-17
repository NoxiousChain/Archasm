extends Node


enum {
	TRUNK_LOW = 10,
	TRUNK = 11,
	TRUNK_TOP = 12,
}

var tiles_tex = {
	TRUNK_TOP = null
}

var unstickable_tiles = [TRUNK_TOP]

var trunk_tiles = [
	TRUNK_TOP
]

var block_stat = {
	TRUNK_TOP: {"hp": 0, "lvl": -1}
}
