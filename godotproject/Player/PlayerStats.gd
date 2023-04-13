extends Resource
class_name PlayerStats

# This script will contain all of the player stat variables.
# All data is saved in "player_stats.tres". 
# This data is loaded into memory in the player script and saved only on save_stats() call.
# Do not read and write to the resource file constantly. This should only happen between scenes
# or when the player saves the game. Otherwise, retrieve and save stats in memory using the stats
# dictionary in the player script.

# IMPORTANT:
# If you add any stats here, they MUST be added to the load, save, get, and set functions in player.gd
# Stats will not be useable otherwise.

var name : String
var max_health : int			# max player health. modified by armor, status effects, etc.
var health : int				# current player health
var attack: float 				# base attack multiplier. modified by armor, status effects, etc.
var defense : int				# defense value

var speed_multi : float			# speed multiplier. modified by status effects, etc.
