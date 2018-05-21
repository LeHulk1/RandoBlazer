## RandoBlazer

This randomizer should be used with the US version of the ROM: 
  Soul Blazer (U) [!].smc
  
### Make a randomized ROM

Your original ROM should have the exact name "Soul Blazer (U) [!].smc", so please rename it if needed. The program should work correctly whether your original ROM is headered or not. Put the original ROM in the same folder as the RandoBlazer executable, and launch it. The program will create the patched ROM (the original will stay intact), a seed file and a Spoiler log. The seed file is just a text file containing the seed which has been used for the randomization (it's just a timestamp, really). If you want to make a randomized ROM from a seed, make a "seed" file yourself with the seed you want to use, before launching the program. The seed should be an integer (not too large, I'm not sure it will work well with more than 10 digits).

### Version 0.4a features

Monster Lair contents:
 - Enemy type is randomized, but will still choose from the possible enemy types in the current location. Monsters which usually don't belong to lairs can be in lairs (like blue blobs and torches in Act 1 Underground Castle).
 - Lair type is randomized: it can be one-by-one spawner, multispawner, or proximity spawner (this one is more rare). The "already there" type is not randomized though.
 - Number of enemies is randomized: between 2 and 6 for one-by-one spawners, and between 4 and 12 for multispawners.
 - Multispawner spawn rate is randomized between 3 and 20 frames.
 
NPCs released from monster lairs are also shuffled in a way that still makes the game beatable. The only exception is King Magridd who is still in the Demon Bird's lair. This is because the boss fight glitches out if the King is already released.

Items from chests and items received from NPCs or found in special places are all shuffled. You can find all non-chest item locations in this pastebin:
https://pastebin.com/aL0ASsYV

"Free" monsters are also randomized, i.e. the monsters which do not belong to a monster lair.

Finally, a lot of text has been edited in order to match the actual item NPCs now give you, but also to reduce the amount of text in most cutscenes. Most revival text has also been edited out. There is also a variety of texts for Master's text in the game introduction and on player's death.

### Planned improvements

 - Make an open mode where the player can travel to all regions right at the start (maybe not World of Evil though).
 - Edit Dr.Leo, Lisa and some other sprites to improve randomization / prevent softlocks.
 - Make Stones actually required to open World of Evil, and randomize town leaders' items.
 - Have varying difficulty levels (changing the item pool, or changing enemy/item stats).
 - More text edits maybe?
 - Improve the general route randomization to make it more balanced.

### Known issues/limitations

 - Dr.Leo's cutscene on the airship can be skipped, which can lead to a softlock if Leo has a required item.
 - A few monster lairs can have "upside-down" enemies which can just escape the level (should not happen on any required lair though).
