extends Node

var item_data: Dictionary

func _ready():
	item_data = LoadData(["res://Data/block_ItemData.json", "res://Data/material_ItemData.json"])
	
func LoadData(file_paths):
	var json_data = {}
	var file_data = File.new()
	
	for file_path in file_paths:
		if file_data.open(file_path, File.READ) == OK:
			json_data[file_path] = JSON.parse(file_data.get_as_text()).result
			file_data.close()
	
	return json_data
