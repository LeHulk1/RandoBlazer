## RandoBlazer

This randomizer should be used with the US version of the ROM: 
  Soul Blazer (U) [!].smc
  
### Make a randomized ROM

Put the original ROM in the same folder as the RandoBlazer executable, and launch it. The program will create the patched ROM (the original will stay intact), a seed file and a Spoiler log. The seed file is just a text file containing the seed which has been used for the randomization (it's just a timestamp, really). If you want to make a randomized ROM from a seed, make a "seed" file yourself with the seed you want to use, before launching the program. The seed should be an integer (not too large, I'm not sure it will work well with more than 10 digits).

### Version 0.2 features

Monster Lair contents:
 - Enemy type is randomized, but will still choose from the possible enemy types in the current location. Monsters which usually don't belong to lairs can be in lairs (like blue blobs and torches in Act 1 Underground Castle).
 - Lair type is randomized: it can be one-by-one spawner, multispawner, or proximity spawner (this one is more rare). The "already there" type is not randomized though.
 - Number of enemies is randomized: between 4 and 8 for one-by-one spawners, and between 6 and 16 for multispawners.
 - Multispawner spawn rate is randomized between 3 and 20 frames.
 
NPCs released from monster lairs are also shuffled in a way that still makes the game beatable. The only exception is King Magridd who is still in the Demon Bird's lair. This is because the boss fight glitches out if the King is already released.

Items from chests are also shuffled, but not the items received from NPCs or found in special places. Therefore, most swords will be found in different locations than in the vanilla game, as well as the Red-Hot Ball. But the other two Red-Hot items will still be given by the same NPCs (however, they won't be released from the same monster lairs!).

### Planned improvements

 - Randomize items from NPCs and special tiles. This will almost double the pool of items!
 - Make an open mode where the player can travel to all regions right at the start (maybe not World of Evil though).
 - Fine-tuning: harmonize EXP required for levels, swords' attack stats, enemy HP, etc.
 - Fix the annoying "upside-down enemy" issue.
 - Randomize the locations of Zantetsu and Spirit swords better

### Known issues/limitations

 - A few monster lairs can have "upside-down" enemies which can just escape the level.
 - Zantetsu and Spirit swords are always near the start of the game, randomization is to be improved.
