extends "res://Inventory/Slot.gd"

const SLOT_TYPE = "CHEST"

func slot_accepts_item(i) -> bool:
	return i.call("get_slot_type") == SLOT_TYPE
