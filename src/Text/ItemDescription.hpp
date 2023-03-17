#pragma once

#include "../Common.hpp"
#include <RichTextLabel.hpp>

// Implements a frame for an item description. This is designed to be displayed in an inventory when an item is hovered over.
// As a child of RichTextLabel, it uses BBCode formatting, with most formatting options supported.
// See https://docs.godotengine.org/en/stable/tutorials/ui/bbcode_in_richtextlabel.html for a list of formatting options.
// There are some nice looking animated options like rainbow that I plan to use for special items
// It is also possible to make custom BBCode tags and effects, see the bottom of the link above (it has some prexisting examples)

// Because BBCode formatting is enabled, do NOT use the += operator to add new text. Instead, use append_text()

namespace godot {

	class ItemDescription : public RichTextLabel {
		GODOT_CLASS(ItemDescription, RichTextLabel)

	public:
		void _init();
		static void _register_methods();
		void build(const String& contents);

		// Display of an item description:

		// Title. Color for this will generally be bound to its rarity. Convert to string with color.to_html(false)	
		// General description (e.g. an axe - "A simple tool used for chopping.")
		// Stats, status effects, attributes, etc. (e.g. "Attack: 9.8\nPoison chance: 6%\nPoison dmg: 4.4/sec")
		// Meta text. This is displayed in color & italics at the bottom of the description.

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

}