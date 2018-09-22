## RandoBlazer

This randomizer should be used with the US version of the ROM: 
  Soul Blazer (U) [!].smc
  
### Make a randomized ROM

Your original ROM should have the exact name "Soul Blazer (U) [!].smc", so please rename it if needed. The program should work correctly whether your original ROM is headered or not. Put the original ROM in the same folder as the RandoBlazer executable, and launch it. The program will create the patched ROM (the original will stay intact), a seed file and a Spoiler log. The seed file is just a text file containing the seed which has been used for the randomization (it's just a timestamp, really). If you want to make a randomized ROM from a seed, make a "seed" file yourself with the seed you want to use, before launching the program. The seed should be an integer (not too large, I'm not sure it will work well with more than 10 digits).

### Version 0.5b features

Monster Lair contents:
 - Enemy type is randomized, but will still choose from the possible enemy types in the current location. Monsters which usually don't belong to lairs can be in lairs (like blue blobs and torches in Act 1 Underground Castle).
 - Lair type is randomized: it can be one-by-one spawner, multispawner, or proximity spawner (this one is more rare). The "already there" type is not randomized though.
 - Number of enemies is randomized: between 2 and 6 for one-by-one spawners, and between 4 and 12 for multispawners.
 - Multispawner spawn rate is randomized between 3 and 20 frames.
 
NPCs released from monster lairs are also shuffled in a way that still makes the game beatable. The progression through the game will probably be quite different from the original game.

Items from chests and items received from NPCs or found in special places are all shuffled. You can find all non-chest item locations in this pastebin:
https://pastebin.com/phaJZdwW

"Free" monsters are also randomized, i.e. the monsters which do not belong to a monster lair. A few of them can have four different possible orientations, and that is randomized too (like the water dragons in Greenwood, or the snow rats in Mountain of Souls).

The Gourmet Goat will give you a clue on the location of either the Soul Blade, the Soul Armor or Phoenix, provided you give it food. Some other NPCs can give clues (the soldier with Dr. Leo, the fairy crystal in Magridd Castle basement, etc.).

The six Stones are required in order to progress to the World of Evil. However, you don't need each individual Stone to progress to the next area: for that you need to talk to the leader NPC of the current area (their item will also be randomized).

Finally, a lot of text has been edited in order to match the actual item NPCs now give you, but also to reduce the amount of text in most cutscenes. Most revival text has also been edited out. There is also a variety of texts for Master's text in the game introduction and on player's death.

### Item/NPC Tracker

You can use this tracker made by Netanonima: https://eggper.com/blazer_tracker/

### Planned improvements

 - Make an open mode where the player can travel to all regions right at the start (maybe not World of Evil though).
 - Have varying difficulty levels (changing the item pool, or changing enemy/item stats).
 - More text edits maybe?
 - Randomize the orientation of enemies in "already there" monster lairs.
 - Improve the general route randomization to make it more balanced.

### Known issues/limitations

