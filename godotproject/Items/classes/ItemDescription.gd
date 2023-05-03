# This class just represents the description of an item. It's basically a wrapper for a RichTextLabel with BBCode
# It should be extended a bit to make it cleaner and maybe make it easier to automatically generate item descriptions
# For example, add functions that automatically convert its stats to the stats section, to automatically color the 
# title based on rarity/keywords, etc.

# See https://docs.godotengine.org/en/stable/tutorials/ui/bbcode_in_richtextlabel.html for a list of formatting options.
# There are some nice looking animated options like rainbow that I planned to use for special items
# It is also possible to make custom BBCode tags and effects, see the bottom of the link above (it has some prexisting examples)

# Because BBCode formatting is enabled, do NOT use the += operator to add new text. Instead, use append_text()

class_name ItemDescription
extends RichTextLabel

func _ready():
	set_use_bbcode(true)
	set_scroll_active(false)
	set_fit_content_height(true)
	

func build(contents : String):
	append_bbcode(contents)



# Display of an item description:

		# Title. Color for this will generally be bound to its rarity. Convert to string with color.to_html(false)	
		# General description (e.g. an axe - "A simple tool used for chopping.")
		# Stats, status effects, attributes, etc. (e.g. "Attack: 9.8\nPoison chance: 6%\nPoison dmg: 4.4/sec")
		# Meta text. This is displayed in color & italics at the bottom of the description.

		# Example of how a full item description would look: (**bold** *italics*)
#		___________________________
#		|   **Infectious Axe**	  | <-- Name
#		|						  | <-- Could insert the item image here as well
#		|	A simple tool used    | <-- Description
#		|   for chopping. A vial  |
#		|	of poison has been    |
#		|	poured on its blade.  |
#		|						  |
#		|						  |
#		|	Attack: 9.8			  | <-- Stats
#		|	Poison chance: 6%	  |
#		|	Poison dmg: 4.4/sec	  |
#		|						  |
#		|						  |
#		|	*The Irish aren't	  |	<-- Meta
#		|	the only ones who	  |
#		|	bleed green.*		  |
#		|_________________________|
