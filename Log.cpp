#include "Log.h"
#include "Map.h"
#include "ROMData.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdio.h>


#define SPOILER_LOG_FILE_NAME "SpoilerLog.txt"
#define NPC_NAME_SIZE         45
#define ITEM_NAME_SIZE        20


using namespace std;

namespace Log {

    static int Act1NPCs[16] =
        {NPC_BRIDGE_GUARD,
         NPC_IVY_CHEST_ROOM,
         NPC_WATER_MILL,
         NPC_ARCHITECT,
         NPC_LEOS_HOUSE,
         NPC_LISA,
         NPC_TOOL_SHOP_OWNER,
         NPC_TEDDY,
         NPC_GOAT_HERB,
         NPC_IVY,
         NPC_IVY_EMBLEM_A,
         NPC_TULIP_PASS,
         NPC_BOY_CAVE,
         NPC_IVY_RECOVERY_SWORD,
         NPC_OLD_WOMAN,
         NPC_VILLAGE_CHIEF};
    static int Act2NPCs[18] =
        {NPC_CROCODILE,
         NPC_CROCODILE2,
         NPC_BIRD_GREENWOOD_LEAF,
         NPC_MOLE_SOUL_OF_LIGHT,
         NPC_MONMO,
         NPC_MOLE3,
         NPC_MOLE,
         NPC_DOG3,
         NPC_DEER,
         NPC_SQUIRREL3,
         NPC_SQUIRREL_PSYCHO_SWORD,
         NPC_SQUIRREL_EMBLEM_C,
         NPC_SQUIRREL_ICE_ARMOR,
         NPC_MOLE_SHIELD_BRACELET,
         NPC_CROCODILE3,
         NPC_DEER_MAGIC_BELL,
         NPC_BIRD_RED_HOT_MIRROR,
         NPC_GREENWOODS_GUARDIAN};
    static int Act3NPCs[16] =
        {NPC_MERMAID_BUBBLE_ARMOR,
         NPC_MERMAID_PEARL,
         NPC_DOLPHIN_PEARL,
         NPC_MERMAID_STATUE_ROCKBIRD,
         NPC_MERMAID_STATUE_DUREAN,
         NPC_MERMAID_STATUE_BLESTER,
         NPC_MERMAID_STATUE_GHOST_SHIP,
         NPC_LUE,
         NPC_DOLPHIN_SAVES_LUE,
         NPC_DOLPHIN_SECRET_CAVE,
         NPC_DOLPHIN2,
         NPC_MERMAID,
         NPC_MERMAID_TEARS,
         NPC_MERMAID_MAGIC_FLARE,
         NPC_MERMAID_RED_HOT_STICK,
         NPC_MERMAID_QUEEN};
    static int Act4NPCs[14] =
        {NPC_BOY_MUSHROOM_SHOES,
         NPC_GIRL3,
         NPC_GRANDPA4,
         NPC_GRANDPA_LUNE,
         NPC_MUSHROOM2,
         NPC_GRANDPA5,
         NPC_SNAIL_EMBLEM_E,
         NPC_MUSHROOM_EMBLEM_F,
         NPC_BOY,
         NPC_GRANDPA3,
         NPC_DANCING_GRANDMA,
         NPC_DANCING_GRANDMA2,
         NPC_NOME,
         NPC_MOUNTAIN_KING};
    static int Act5NPCs[17] =
        {NPC_GREAT_DOOR_ZANTETSU_SWORD,
         NPC_STEPS_UPSTAIRS,
         NPC_GREAT_DOOR_MODEL_TOWNS,
         NPC_MODEL_TOWN1,
         NPC_MODEL_TOWN2,
         NPC_STAIRS_POWER_PLANT,
         NPC_STEPS_MARIE,
         NPC_CAT,
         NPC_CAT2,
         NPC_MOUSE,
         NPC_MOUSE_SPARK_BOMB,
         NPC_PLANT_HERB,
         NPC_PLANT_ACTINIDIA_LEAVES,
         NPC_CAT_DOOR_KEY,
         NPC_GREAT_DOOR,
         NPC_CHEST_OF_DRAWERS_MYSTIC_ARMOR,
         NPC_MARIE};
    static int Act6NPCs[12] =
        {NPC_SINGER_CONCERT_HALL,
         NPC_SOLDIER_PLATINUM_CARD,
         NPC_SOLDIER_LEFT_TOWER,
         NPC_QUEEN_MAGRIDD,
         NPC_SOLDIER_RIGHT_TOWER,
         NPC_SOLDIER_CASTLE,
         NPC_SOLDIER_WITH_LEO,
         NPC_SOLDIER_DOK,
         NPC_DR_LEO,
         NPC_SOLDIER_ELEMENTAL_MAIL,
         NPC_MAID_HERB,
         NPC_KING_MAGRIDD};

    static int LoggedItems[62] =
        {SWORD_OF_LIFE,
         PSYCHO_SWORD,
         CRITICAL_SWORD,
         LUCKY_BLADE,
         ZANTETSU_SWORD,
         SPIRIT_SWORD,
         RECOVERY_SWORD,
         SOUL_BLADE,
         IRON_ARMOR,
         ICE_ARMOR,
         BUBBLE_ARMOR,
         MAGIC_ARMOR,
         MYSTIC_ARMOR,
         LIGHT_ARMOR,
         ELEMENTAL_MAIL,
         SOUL_ARMOR,
         FLAME_BALL,
         LIGHT_ARROW,
         MAGIC_FLARE,
         ROTATOR,
         SPARK_BOMB,
         FLAME_PILLAR,
         TORNADO,
         PHOENIX,
         GOATS_FOOD,
         HARP_STRING,
         PASS,
         DREAM_ROD,
         LEOS_BRUSH,
         GREENWOOD_LEAF,
         MOLES_RIBBON,
         BIG_PEARL,
         MERMAIDS_TEARS,
         MUSHROOM_SHOES,
         MOBILE_KEY,
         THUNDER_RING,
         DELICIOUS_SEEDS,
         ACTINIDIA_LEAF,
         DOOR_KEY,
         PLATINUM_CARD,
         VIP_CARD,
         EMBLEM_A,
         EMBLEM_B,
         EMBLEM_C,
         EMBLEM_D,
         EMBLEM_E,
         EMBLEM_F,
         EMBLEM_G,
         EMBLEM_H,
         RED_HOT_MIRROR,
         RED_HOT_BALL,
         RED_HOT_STICK,
         POWER_BRACELET,
         SHIELD_BRACELET,
         SUPER_BRACELET,
         BROWN_STONE,
         GREEN_STONE,
         BLUE_STONE,
         SILVER_STONE,
         PURPLE_STONE,
         BLACK_STONE,
         MAGIC_BELL};
    static string LoggedItemNames[62] =
        {"Sword of Life",
         "Psycho Sword",
         "Critical Sword",
         "Lucky Blade",
         "Zantetsu Sword",
         "Spirit Sword",
         "Recovery Sword",
         "Soul Blade",
         "Iron Armor",
         "Ice Armor",
         "Bubble Armor",
         "Magic Armor",
         "Mystic Armor",
         "Light Armor",
         "Elemental Mail",
         "Soul Armor",
         "Flame Ball",
         "Light Arrow",
         "Magic Flare",
         "Rotator",
         "Spark Bomb",
         "Flame Pillar",
         "Tornado",
         "Phoenix",
         "Goat's Food",
         "Harp String",
         "Pass",
         "Dream Rod",
         "Leo's Brush",
         "Greenwood Leaf",
         "Mole's Ribbon",
         "Big Pearl",
         "Mermaid's Tears",
         "Mushroom Shoes",
         "Mobile Key",
         "Thunder Ring",
         "Delicious Seeds",
         "Actinidia Leaf",
         "Door Key",
         "Platinum Card",
         "VIP Card",
         "Emblem A",
         "Emblem B",
         "Emblem C",
         "Emblem D",
         "Emblem E",
         "Emblem F",
         "Emblem G",
         "Emblem H",
         "Red-Hot Mirror",
         "Red-Hot Ball",
         "Red-Hot Stick",
         "Power Bracelet",
         "Shield Bracelet",
         "Super Bracelet",
         "Brown Stone",
         "Green Stone",
         "Blue Stone",
         "Silver Stone",
         "Purple Stone",
         "Black Stone",
         "Magic Bell"};
    static string ItemLocations[NUMBER_OF_ITEMS] =
        {"Trial Room at the start",
         "Grass Valley secret cave (left chest)",
         "Grass Valley secret cave (right chest)",
         "Underground Castle (12-gem chest)",
         "Underground Castle (Herb chest)",
         "Underground Castle (Dream Rod chest)",
         "Underground Castle (Leo's Brush chest)",
         "Leo's Paintings (Herb chest)",
         "Leo's Paintings (Tornado chest)",
         "Greenwood (Ice Armor chest)",
         "Greenwood tunnels (Mole's Ribbon chest)",
         "Water Shrine 1st screen (50-gem chest)",
         "Water Shrine 2nd screen (north, 150-gem chest)",
         "Water Shrine 2nd screen (Herb chest)",
         "Water Shrine 3rd screen (south-west chest)",
         "Water Shrine 3rd screen (south-east chest)",
         "Fire Shrine 1st screen (Herb chest)",
         "Fire Shrine 2nd screen disappearing chest",
         "Fire Shrine 2nd screen (Scorpions' Lair)",
         "Fire Shrine 3rd screen (100-gem chest)",
         "Fire Shrine 3rd screen (60-gem chest)",
         "Light Shrine (Flame Pillar chest)",
         "St Elles (Mermaid's Tears chest)",
         "St Elles (Big Pearl chest)",
         "Seabed secret cave (top-left chest)",
         "Seabed secret cave (top-right chest)",
         "Seabed secret cave (bottom-left chest)",
         "Seabed secret cave (bottom-right chest)",
         "Southerta (Herb chest)",
         "Rockbird (Herb chest)",
         "Rockbird (60-gem chest)",
         "Durean (Critical Sword chest)",
         "Durean (Strange Bottle chest)",
         "Ghost Ship (1-gem chest)",
         "Seabed, way to Ghost Ship (Power Bracelet chest)",
         "Mountain of Souls 1st screen (40-gem chest)",
         "Mountain of Souls 2nd screen (left-most chest)",
         "Mountain of Souls 2nd screen (2nd chest from the left)",
         "Mountain of Souls 2nd screen (2nd chest from the right)",
         "Mountain of Souls 2nd screen (right-most chest)",
         "Laynole 2nd screen (Lucky Blade chest)",
         "Laynole 2nd screen (north-east, Herb chest)",
         "Laynole 2nd screen (north-east, Rotator chest)",
         "Leo's Lab (Zantetsu Sword chest)",
         "Leo's Lab Power Plant (Light Armor chest)",
         "Model Town 1 (south-east, 50-gem chest)",
         "Model Town 1 (north part, left chest)",
         "Model Town 1 (north part, right chest)",
         "Model Town 2 (top chest)",
         "Model Town 2 (bottom chest)",
         "Magridd Castle Basement 1st screen (west chest)",
         "Magridd Castle Basement 1st screen (Spirit Sword chest)",
         "Magridd Castle Basement 2nd screen (north chest)",
         "Magridd Castle Basement 2nd screen (south-west chest)",
         "Magridd Castle Basement 2nd screen (middle, 100-gem chest)",
         "Magridd Castle Basement 3rd screen (north-west chest)",
         "Magridd Castle Right Tower 2nd screen (left chest)",
         "Magridd Castle Right Tower 2nd screen (right chest)",
         "Magridd Castle Right Tower 3rd screen (top-left chest)",
         "Magridd Castle Right Tower 3rd screen (bottom-right chest)",
         "World of Evil 1st screen (south-east chest)",
         "World of Evil 1st screen (south-west chest)",
         "World of Evil 1st screen (Red-Hot Ball chest)",
         "World of Evil 2nd screen (Soul Armor chest)",
         "Dazzling Space (south-east, Soul Blade chest)",
         "Dazzling Space (south-west, 100-gem chest)",
         "Tool Shop Owner's item",
         "Emblem A tile (south-east of Grass Valley)",
         "Goat Pen (bottom-right corner tile)",
         "Teddy's item (Tool Shop)",
         "Item under sleeping Tulip (south-east of Grass Valley)",
         "Secret hideout tile (to the right after exiting the grotto)",
         "Village Chief's item",
         "Magician's item (start of the game)",
         "Recovery Sword crystal fairy (in secret hideout)",
         "Crystal fairy in 2-chest room of Grass Valley",
         "First crystal fairy in Underground Castle",
         "Red-Hot Mirror bird",
         "Magic Bell crystal fairy (requires all 8 emblems)",
         "Woodstin Trio's prize",
         "Greenwood's Guardian's item",
         "Greenwood Leaf tile (under Turbo's bones)",
         "Shield Bracelet mole's item",
         "Psycho Sword squirrel's item (requires Delicious Seeds)",
         "Emblem C squirrel's item",
         "Water Shrine basement's Strange Bottle tile",
         "Light Arrow crystal fairy in Fire Shrine",
         "Lost Marsh crystal fairy (near Light Shrine entrance)",
         "Water Shrine 2nd screen crystal fairy",
         "Fire Shrine 1st screen crystal fairy",
         "Mountain King's item",
         "Mushroom Shoes boy's item",
         "Nome's item",
         "Emblem E snail's item (in secret room)",
         "Emblem F tile in Lune (unlocked in mushroom's dream)",
         "Mountain of Souls 3rd screen crystal fairy",
         "Lune crystal fairy",
         "Under Chest of Drawers in locked room",
         "Inside Chest of Drawers in locked room",
         "Medical Herb plant in Leo's Lab main room",
         "Leo's Cat's item",
         "Actinidia Leaf plant's item",
         "Marie's item",
         "Spark Bomb Mouse's item (in mouse hole)",
         "Leo's Lab basement 2nd screen crystal fairy",
         "Model Town 1 crystal fairy",
         "Leo's Lab Power Plant crystal fairy",
         "Sleeping Soldier's item",
         "Super Bracelet tile in Queen Magridd's house",
         "Queen Magridd's item",
         "Platinum Card Soldier's item (in concert hall)",
         "Medical Herb Maid's item",
         "Emblem H tile outside Magridd Castle",
         "King Magridd's item",
         "Dr. Leo's item",
         "Harp String tile (Magridd Castle Basement 1st screen)",
         "Medical Herb Mermaid (St. Elles north-eastern house)",
         "Bubble Armor Mermaid's item",
         "Magic Flare Mermaid's item (south-west of St. Elles)",
         "Mermaid Queen's item",
         "Red-Hot Stick Mermaid's item",
         "Lue's item",
         "Crystal fairy in Rockbird",
         "Seabed crystal fairy (near Blester entrance)",
         "Seabed crystal fairy (near Durean entrance)"};

    int GetAreaNumber(Lair &Lair) {
        switch (Lair.Act) {
        case 0x00:
            return 1;
        case 0x01:
            return 2;
        case 0x02:
            return 3;
        case 0x03:
            return 4;
        case 0x04:
            return 5;
        case 0x05:
            return 6;
        default:
            /* Should not happen */
            return 0;
        }
    }

    string GetKeyNPCName(int NPC_ID) {
        switch (NPC_ID) {
        case NPC_BRIDGE_GUARD:
            return "Bridge Guard";
        case NPC_IVY_CHEST_ROOM:
            return "Ivy (leads to the 2-chest room)";
        case NPC_WATER_MILL:
            return "Watermill Keeper";
        case NPC_ARCHITECT:
            return "Architect (stairs to Leo's house)";
        case NPC_LEOS_HOUSE:
            return "Old Man (Leo's house with painting)";
        case NPC_LISA:
            return "Lisa";
        case NPC_TOOL_SHOP_OWNER:
            return "Tool Shop Owner";
        case NPC_TEDDY:
            return "Teddy (Tool Shop Owner's son)";
        case NPC_GOAT_HERB:
            return "Goat (unlocks Goat Pen)";
        case NPC_IVY:
            return "Ivy (to south-eastern Grass Valley)";
        case NPC_IVY_EMBLEM_A:
            return "Ivy (to Emblem A tile)";
        case NPC_TULIP_PASS:
            return "Tulip (push for Pass)";
        case NPC_BOY_CAVE:
            return "Boy (guards Grass Valley hideout)";
        case NPC_IVY_RECOVERY_SWORD:
            return "Ivy (to Recovery Sword crystal)";
        case NPC_OLD_WOMAN:
            return "Old Woman (Village Chief's house)";
        case NPC_VILLAGE_CHIEF:
            return "Village Chief (leads to Act 2)";
        case NPC_CROCODILE:
            return "First Crocodile (to sleepy bird)";
        case NPC_CROCODILE2:
            return "Second Crocodile (to sleepy bird)";
        case NPC_BIRD_GREENWOOD_LEAF:
            return "Sleepy Bird (unlocks Greenwood Leaf)";
        case NPC_MOLE_SOUL_OF_LIGHT:
            return "Mole / Soul of Light (Greenwood Leaf)";
        case NPC_MONMO:
            return "Monmo the Mole";
        case NPC_MOLE3:
            return "Monmo's hole";
        case NPC_MOLE:
            return "Hole leading to dreaming stump";
        case NPC_DOG3:
            return "Woodstin Trio dog";
        case NPC_DEER:
            return "Woodstin Trio deer";
        case NPC_SQUIRREL3:
            return "Woodstin Trio squirrel";
        case NPC_SQUIRREL_PSYCHO_SWORD:
            return "Squirrel (trades Psycho Sword)";
        case NPC_SQUIRREL_EMBLEM_C:
            return "Squirrel (gives Emblem C)";
        case NPC_SQUIRREL_ICE_ARMOR:
            return "Squirrel (leads to Ice Armor chest)";
        case NPC_MOLE_SHIELD_BRACELET:
            return "Mole (gives Shield Bracelet)";
        case NPC_CROCODILE3:
            return "Crocodile (to Deer)";
        case NPC_DEER_MAGIC_BELL:
            return "Deer (unlocks Magic Bell crystal)";
        case NPC_BIRD_RED_HOT_MIRROR:
            return "Bird (gives Red-Hot Mirror)";
        case NPC_GREENWOODS_GUARDIAN:
            return "Greenwood's Guardian (leads to Act 3)";
        case NPC_MERMAID_BUBBLE_ARMOR:
            return "Mermaid (gives Bubble Armor)";
        case NPC_MERMAID_PEARL:
            return "Mermaid (unlocks Lue's prison)";
        case NPC_DOLPHIN_PEARL:
            return "Dolphin (leading to Big Pearl chest)";
        case NPC_MERMAID_STATUE_ROCKBIRD:
            return "Mermaid Statue (leading to Rockbird)";
        case NPC_MERMAID_STATUE_DUREAN:
            return "Mermaid Statue (leading to Durean)";
        case NPC_MERMAID_STATUE_BLESTER:
            return "Mermaid Statue (leading to Blester)";
        case NPC_MERMAID_STATUE_GHOST_SHIP:
            return "Mermaid Statue (leading to Ghost Ship)";
        case NPC_LUE:
            return "Lue (gives Thunder Ring)";
        case NPC_DOLPHIN_SAVES_LUE:
            return "Dolphin (saves Lue from prison)";
        case NPC_DOLPHIN_SECRET_CAVE:
            return "Sleeping Dolphin (unlocks secret cave)";
        case NPC_DOLPHIN2:
            return "Dolphin (unlocks north-eastern platform)";
        case NPC_MERMAID:
            return "Mermaid (gives Medical Herb)";
        case NPC_MERMAID_TEARS:
            return "Mermaid by the Mermaid Tears chest";
        case NPC_MERMAID_MAGIC_FLARE:
            return "Mermaid (gives Magic Flare)";
        case NPC_MERMAID_RED_HOT_STICK:
            return "Mermaid (gives Red-Hot Stick)";
        case NPC_MERMAID_QUEEN:
            return "Mermaid Queen (leads to Act 4)";
        case NPC_BOY_MUSHROOM_SHOES:
            return "Boy (gives Mushroom Shoes)";
        case NPC_GIRL3:
            return "Girl (unlocks eastern tunnel)";
        case NPC_GRANDPA4:
            return "Grandpa (unlocks south-eastern lake)";
        case NPC_GRANDPA_LUNE:
            return "Grandpa (leads to Lune)";
        case NPC_MUSHROOM2:
            return "Mushroom (unlocks southern tunnel)";
        case NPC_GRANDPA5:
            return "Grandpa (unlocks Nome's prison)";
        case NPC_SNAIL_EMBLEM_E:
            return "Snail in secret room (gives Emblem E)";
        case NPC_MUSHROOM_EMBLEM_F:
            return "Sleeping Mushroom (unlocks lair in Lune)";
        case NPC_BOY:
            return "Boy (unlocks western tunnel)";
        case NPC_GRANDPA3:
            return "Grandpa (unlocks south-western tunnel)";
        case NPC_DANCING_GRANDMA:
            return "Dancing Grandma 1";
        case NPC_DANCING_GRANDMA2:
            return "Dancing Grandma 2";
        case NPC_NOME:
            return "Nome (leads to Act 5)";
        case NPC_MOUNTAIN_KING:
            return "Mountain King (gives Phoenix)";
        case NPC_GREAT_DOOR_ZANTETSU_SWORD:
            return "Great Door (to Zantetsu Sword chest)";
        case NPC_STEPS_UPSTAIRS:
            return "Steps (to second floor)";
        case NPC_GREAT_DOOR_MODEL_TOWNS:
            return "Great Door (to Model Town room)";
        case NPC_MODEL_TOWN1:
            return "Model Town 1";
        case NPC_MODEL_TOWN2:
            return "Model Town 2";
        case NPC_STAIRS_POWER_PLANT:
            return "Stairs (to Power Plant)";
        case NPC_STEPS_MARIE:
            return "Steps (to Marie's attic)";
        case NPC_CAT:
            return "First Cat in Leo's Lab main room";
        case NPC_CAT2:
            return "Second Cat in Leo's Lab main room";
        case NPC_MOUSE:
            return "Mouse (unlocks Mouse hole)";
        case NPC_MOUSE_SPARK_BOMB:
            return "Mouse (gives Spark Bomb)";
        case NPC_PLANT_HERB:
            return "Plant (gives Medical Herbs)";
        case NPC_PLANT_ACTINIDIA_LEAVES:
            return "Plant (gives Actinidia Leaves)";
        case NPC_CAT_DOOR_KEY:
            return "Leo's sleeping Cat (gives Door Key)";
        case NPC_GREAT_DOOR:
            return "Locked Door to south-western room";
        case NPC_CHEST_OF_DRAWERS_MYSTIC_ARMOR:
            return "Chest of Drawers (gives Mystic Armor)";
        case NPC_MARIE:
            return "Marie (leads to Act 6)";
        case NPC_SINGER_CONCERT_HALL:
            return "Singer (unlocks concert hall)";
        case NPC_SOLDIER_PLATINUM_CARD:
            return "Soldier (has Platinum Card)";
        case NPC_SOLDIER_LEFT_TOWER:
            return "Soldier (guarding Left Tower)";
        case NPC_QUEEN_MAGRIDD:
            return "Queen Magridd (gives VIP Card)";
        case NPC_SOLDIER_RIGHT_TOWER:
            return "Soldier (guarding Right Tower)";
        case NPC_SOLDIER_CASTLE:
            return "Soldier (unlocks Magridd Castle)";
        case NPC_SOLDIER_WITH_LEO:
            return "Soldier (accompanies Dr. Leo)";
        case NPC_SOLDIER_DOK:
            return "Soldier (Dok; accompanies Dr. Leo)";
        case NPC_DR_LEO:
            return "Dr. Leo";
        case NPC_SOLDIER_ELEMENTAL_MAIL:
            return "Sleeping Soldier (gives Elemental Mail)";
        case NPC_MAID_HERB:
            return "Maid (gives Medical Herbs)";
        case NPC_KING_MAGRIDD:
            return "King Magridd (leads to Act 7)";
        default:
            /* Should not happen */
            return "";
        }
    }

    string GetAreaName(Lair &Lair) {
        switch (Lair.PositionData[0]) {
        case 0x05:
            return "Underground Castle 1st screen";
        case 0x06:
            return "Underground Castle 2nd screen";
        case 0x07:
            return "Leo's Paintings 1st screen";
        case 0x08:
            return "Leo's Paintings 2nd screen";
        case 0x09:
            return "Leo's Paintings 3rd screen";
        case 0x0B:
            return "Leo's Paintings 4th screen";
        case 0x0C:
            return "Solid Arm's Lair";
        case 0x0D:
            return "Trial Room (start of the game)";
        case 0x19:
            return "Lost Marshes";
        case 0x1A:
            return "Water Shrine 1st screen";
        case 0x1B:
            return "Water Shrine 2nd screen";
        case 0x1C:
            return "Water Shrine 3rd screen";
        case 0x1D:
            return "Fire Shrine 1st screen";
        case 0x1E:
            return "Fire Shrine 2nd screen";
        case 0x1F:
            return "Fire Shrine 3rd screen";
        case 0x20:
            return "Light Shrine 1st screen";
        case 0x21:
            return "Light Shrine 2nd screen";
        case 0x22:
            return "Elemental Statues' Lair";
        case 0x2A:
            return "Seabed (south screen)";
        case 0x2E:
            return "Southerta";
        case 0x2F:
            return "Rockbird";
        case 0x30:
            return "Durean";
        case 0x31:
            return "Blester";
        case 0x32:
            return "Floating Skull's Lair";
        case 0x34:
            return "Seabed (north screen)";
        case 0x3E:
            return "Mountain of Souls 1st screen";
        case 0x3F:
            return "Mountain of Souls 2nd screen";
        case 0x41:
            return "Lune 1st screen";
        case 0x42:
            return "Lune 2nd screen";
        case 0x44:
            return "Poseidon's Lair";
        case 0x45:
            return "Mountain of Souls 3rd screen";
        case 0x46:
            return "Laynole 1st screen";
        case 0x47:
            return "Laynole 2nd screen";
        case 0x56:
            return "Leo's Lab Basement 1st screen";
        case 0x57:
            return "Leo's Lab Basement 2nd screen";
        case 0x58:
            return "Leo's Lab Power Plant";
        case 0x59:
            return "Tin Doll's Lair";
        case 0x5B:
            return "Leo's Lab Model Town 1";
        case 0x5C:
            return "Leo's Lab Model Town 2";
        case 0x66:
            return "Magridd Castle Basement 1st screen";
        case 0x67:
            return "Magridd Castle Basement 2nd screen";
        case 0x68:
            return "Magridd Castle Basement 3rd screen";
        case 0x6A:
            return "Magridd Castle Left Tower 1st screen";
        case 0x6B:
            return "Magridd Castle Left Tower 2nd screen";
        case 0x6C:
            return "Magridd Castle Left Tower 3rd screen";
        case 0x6D:
            return "Magridd Castle Right Tower 1st screen";
        case 0x6E:
            return "Magridd Castle Right Tower 2nd screen";
        case 0x6F:
            return "Magridd Castle Right Tower 3rd screen";
        case 0x72:
            return "Demon Bird's Lair";
        default:
            /* Should not happen */
            return "";
        }
    }


    void CreateSpoilerLog(vector<Lair>  &RandomizedLairList,
                          vector<Item>  &RandomizedItemList) {

        ofstream LogFile(SPOILER_LOG_FILE_NAME);
        int      NPCIndex, ItemIndex, LoggedItemIndex;

        LogFile << endl;
        LogFile << "     /=============================\\" << endl;
        LogFile << "     |   RandoBlazer Spoiler Log   |"  << endl;
        LogFile << "     \\=============================/" << endl;
        LogFile << endl;
        LogFile << endl;


        LogFile << "  ----- KEY NPCs -----" << endl;
        LogFile << endl;

        LogFile << "Act 1: Grass Valley" << endl;
        for (NPCIndex = 0; NPCIndex < 16; NPCIndex++) {
            LogFile << setw(NPC_NAME_SIZE) << left << GetKeyNPCName(Act1NPCs[NPCIndex])
                    << "  ----->  Act " << GetAreaNumber(RandomizedLairList[Act1NPCs[NPCIndex]])
                    << " - " << GetAreaName(RandomizedLairList[Act1NPCs[NPCIndex]]) << endl;
        }
        LogFile << endl;

        LogFile << "Act 2: Greenwood" << endl;
        for (NPCIndex = 0; NPCIndex < 18; NPCIndex++) {
            LogFile << setw(NPC_NAME_SIZE) << left << GetKeyNPCName(Act2NPCs[NPCIndex])
                    << "  ----->  Act " << GetAreaNumber(RandomizedLairList[Act2NPCs[NPCIndex]])
                    << " - " << GetAreaName(RandomizedLairList[Act2NPCs[NPCIndex]]) << endl;
        }
        LogFile << endl;

        LogFile << "Act 3: St Elles" << endl;
        for (NPCIndex = 0; NPCIndex < 16; NPCIndex++) {
            LogFile << setw(NPC_NAME_SIZE) << left << GetKeyNPCName(Act3NPCs[NPCIndex])
                    << "  ----->  Act " << GetAreaNumber(RandomizedLairList[Act3NPCs[NPCIndex]])
                    << " - " << GetAreaName(RandomizedLairList[Act3NPCs[NPCIndex]]) << endl;
        }
        LogFile << endl;

        LogFile << "Act 4: Mountain of Souls" << endl;
        for (NPCIndex = 0; NPCIndex < 14; NPCIndex++) {
            LogFile << setw(NPC_NAME_SIZE) << left << GetKeyNPCName(Act4NPCs[NPCIndex])
                    << "  ----->  Act " << GetAreaNumber(RandomizedLairList[Act4NPCs[NPCIndex]])
                    << " - " << GetAreaName(RandomizedLairList[Act4NPCs[NPCIndex]]) << endl;
        }
        LogFile << endl;

        LogFile << "Act 5: Leo's Lab" << endl;
        for (NPCIndex = 0; NPCIndex < 17; NPCIndex++) {
            LogFile << setw(NPC_NAME_SIZE) << left << GetKeyNPCName(Act5NPCs[NPCIndex])
                    << "  ----->  Act " << GetAreaNumber(RandomizedLairList[Act5NPCs[NPCIndex]])
                    << " - " << GetAreaName(RandomizedLairList[Act5NPCs[NPCIndex]]) << endl;
        }
        LogFile << endl;

        LogFile << "Act 6: Magridd Castle" << endl;
        for (NPCIndex = 0; NPCIndex < 12; NPCIndex++) {
            LogFile << setw(NPC_NAME_SIZE) << left << GetKeyNPCName(Act6NPCs[NPCIndex])
                    << "  ----->  Act " << GetAreaNumber(RandomizedLairList[Act6NPCs[NPCIndex]])
                    << " - " << GetAreaName(RandomizedLairList[Act6NPCs[NPCIndex]]) << endl;
        }
        LogFile << endl;
        LogFile << endl;


        LogFile << "  ----- EQUIPMENT / KEY ITEMS -----" << endl;
        LogFile << endl;

        for (LoggedItemIndex = 0; LoggedItemIndex < 62; LoggedItemIndex++) {
            for (ItemIndex = 0; ItemIndex < NUMBER_OF_ITEMS; ItemIndex++) {
                /* This search method is very inefficient but who cares */
                if (RandomizedItemList[ItemIndex].Contents == LoggedItems[LoggedItemIndex]) {
                    break;
                }
            }

            LogFile << setw(ITEM_NAME_SIZE) << left << LoggedItemNames[LoggedItemIndex]
                    << "  ----->  " << ItemLocations[ItemIndex] << endl;
        }

        LogFile.close();
    }
}
