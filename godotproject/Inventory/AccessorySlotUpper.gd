extends "res://Inventory/Slot.gd"

const SLOT_TYPE = "ACCESSORY_UPPER"

func slot_accepts_item(i) -> bool:
	return i.call("get_slot_type") == SLOT_TYPE
