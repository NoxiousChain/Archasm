#pragma once

#include "../Common.hpp"
#include <RichTextLabel.hpp>
#include <vector>

class ItemDescription : public RichTextLabel {
	GODOT_CLASS(ItemDescription, RichTextLabel)

public:
	void _init();
	static void _register_methods();
	void _ready();

	Color color;				// Title color. This will generally be bound to its rarity.
	String description;			// General description (e.g. an axe - "A simple tool used for chopping.")
	vector<String> stats;		// Stats, status effects, attributes, etc. (e.g. "Attack: 9.8\nPoison chance: 6%\nPoison dmg: 4.4/sec")
	String meta;				// Meta text. This is displayed in color at the bottom of the description.

	// Example of how a full item description would look: (**bold** *italics*)
	/*
	___________________________
	|   **Infectious Axe**	  | <-- Name
	|						  |
	|	A simple tool used    | <-- Description
	|   for chopping. A vial  |
	|	of poison has been    |
	|	poured on its blade.  |
	|						  |
	|						  |
	|	Attack: 9.8			  | <-- Stats
	|	Poison chance: 6%	  |
	|	Poison dmg: 4.4/sec	  |
	|						  |
	|						  |
	|	*The Irish aren't	  |	<-- Meta
	|	the only ones who	  |
	|	bleed green.*		  |
	|_________________________|
	
	*/
};