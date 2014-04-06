#include "stdafx.h"
// #include "libtcod.hpp"
#include <sstream>
#include <fstream>
#include <typeinfo>
#include <cstdlib>
#include <unordered_map>

#include <windows.h>

#include "input.h"
#include "game.h"
#include <actors\Person.h>
#include "map.h"
#include "utils.h"
#include "inventory.h"
#include "Representation.h"
#include "tile.h"	
#include "equipment.h"
#include "ui.h"
#include "item.h"
#include "debug_options.h"
#include "combat.h"
#include "attribute_container.h"
#include "attribute.h"
#include "enums\screens.h"
#include "spells.h"
#include "item_effect.h"
#include "class.h"


enum basic_cmds_t {
    Pickup, Drop,
    OpenInventory,
    Look, ActivateDoor,
    DownStairs, UpStairs,
    OpenMagic, ConfirmCast,
    OpenCharacterSheet,
    OpenHelp, OpenClassSelect,
    NO_MATCHING_BASIC_CMD
};

basic_cmds_t  basic_cmd_pressed(TCOD_key_t key)
{
    std::map<int, basic_cmds_t> spec_movemap; //Keypad, punctuation
    std::map<char, basic_cmds_t> char_movemap; //regular letters

    // spec_movemap[','] = basic_cmds_t::Pickup;
    // spec_movemap[TCODK_KP8] = 

    char_movemap['i'] = basic_cmds_t::OpenInventory;
    char_movemap['c'] = basic_cmds_t::OpenCharacterSheet;
    char_movemap['?'] = basic_cmds_t::OpenHelp;
    char_movemap[','] = basic_cmds_t::Pickup;
    char_movemap['o'] = basic_cmds_t::ActivateDoor;
    char_movemap['>'] = basic_cmds_t::DownStairs;
    char_movemap['<'] = basic_cmds_t::UpStairs;
    char_movemap['m'] = basic_cmds_t::OpenMagic;
    char_movemap['k'] = basic_cmds_t::ConfirmCast;
    char_movemap['p'] = basic_cmds_t::OpenClassSelect;

    if (key.vk == TCODK_CHAR) 
    {
        auto it = char_movemap.find(key.c);
        if(it == char_movemap.end())
        {
            return basic_cmds_t::NO_MATCHING_BASIC_CMD;
        }
        return it->second;
    }
    else
    {
        auto it = spec_movemap.find(key.vk);
        if(it == spec_movemap.end())
        {
            return basic_cmds_t::NO_MATCHING_BASIC_CMD;
        }
        return it->second;
    }

};

enum inventory_items_active_t {
    ExamineItem,
    EquipItem, UnequipItem,
    DropItem, EscapeMenuItem,
    UseItem,
    NO_MATCHING_ITEMS_ACTIVE
};

enum spells_active_t {
    ExamineSpell,
    EquipSpell, UnequipSpell,
    DropSpell, EscapeMenuSpell,
    CastSpell,
    NO_MATCHING_SPELLS_ACTIVE
};

enum classes_active_t {
    ExamineClass,
    EquipClass, UnequipClass,
    DropClass, EscapeMenuClass,
    CastClass,
    NO_MATCHING_CLASSES_ACTIVE
};

spells_active_t spells_active_pressed(TCOD_key_t key)
{
    std::map<int, spells_active_t> spec_spellactivemap; //Keypad, punctuation
    std::map<char, spells_active_t> char_spellactivemap; //regular letters
    // 
    spec_spellactivemap[TCODK_ESCAPE] = spells_active_t::EscapeMenuSpell;
    //     spec_spellactivemap['\''] = spells_active_t::N;

    char_spellactivemap['x'] = spells_active_t::ExamineSpell;
    // char_spellactivemap['d'] = spells_active_t::DropSpell;
    char_spellactivemap['c'] = spells_active_t::CastSpell;
    // char_spellactivemap['e'] = spells_active_t::EquipSpell;
    // char_spellactivemap['y'] = spells_active_t::UnequipSpell;
    char_spellactivemap['q'] = spells_active_t::EscapeMenuSpell;

    if (key.vk == TCODK_CHAR) 
    {
        auto it = char_spellactivemap.find(key.c);
        if(it == char_spellactivemap.end())
        {
            return spells_active_t::NO_MATCHING_SPELLS_ACTIVE;
        }
        return it->second;
    }
    else
    {
        auto it = spec_spellactivemap.find(key.vk);
        if(it == spec_spellactivemap.end())
        {
            return spells_active_t::NO_MATCHING_SPELLS_ACTIVE;
        }
        return it->second;
    }
};

classes_active_t classes_active_pressed(TCOD_key_t key)
{
    std::map<int, classes_active_t> spec_classactivemap; //Keypad, punctuation
    std::map<char, classes_active_t> char_classactivemap; //regular letters
    // 
    spec_classactivemap[TCODK_ESCAPE] = classes_active_t::EscapeMenuClass;
    //     spec_classactivemap['\''] = classes_active_t::N;

    char_classactivemap['x'] = classes_active_t::ExamineClass;
    // char_classactivemap['d'] = classes_active_t::Dropclass;
    char_classactivemap['c'] = classes_active_t::CastClass;
    char_classactivemap['e'] = classes_active_t::EquipClass;
    // char_classactivemap['y'] = classes_active_t::Unequipclass;
    char_classactivemap['q'] = classes_active_t::EscapeMenuClass;

    if (key.vk == TCODK_CHAR) 
    {
        auto it = char_classactivemap.find(key.c);
        if(it == char_classactivemap.end())
        {
            return classes_active_t::NO_MATCHING_CLASSES_ACTIVE;
        }
        return it->second;
    }
    else
    {
        auto it = spec_classactivemap.find(key.vk);
        if(it == spec_classactivemap.end())
        {
            return classes_active_t::NO_MATCHING_CLASSES_ACTIVE;
        }
        return it->second;
    }
};

inventory_items_active_t inventory_items_active_pressed(TCOD_key_t key)
{
    std::map<int, inventory_items_active_t> spec_invitemactivemap; //Keypad, punctuation
    std::map<char, inventory_items_active_t> char_invitemactivemap; //regular letters
    // 
    spec_invitemactivemap[TCODK_ESCAPE] = inventory_items_active_t::EscapeMenuItem;
    //     spec_invitemactivemap['\''] = inventory_items_active_t::N;

    char_invitemactivemap['x'] = inventory_items_active_t::ExamineItem;
    char_invitemactivemap['d'] = inventory_items_active_t::DropItem;
    char_invitemactivemap['u'] = inventory_items_active_t::UseItem;
    char_invitemactivemap['e'] = inventory_items_active_t::EquipItem;
    char_invitemactivemap['y'] = inventory_items_active_t::UnequipItem;
    char_invitemactivemap['q'] = inventory_items_active_t::EscapeMenuItem;

    if (key.vk == TCODK_CHAR) 
    {
        auto it = char_invitemactivemap.find(key.c);
        if(it == char_invitemactivemap.end())
        {
            return inventory_items_active_t::NO_MATCHING_ITEMS_ACTIVE;
        }
        return it->second;
    }
    else
    {
        auto it = spec_invitemactivemap.find(key.vk);
        if(it == spec_invitemactivemap.end())
        {
            return inventory_items_active_t::NO_MATCHING_ITEMS_ACTIVE;
        }
        return it->second;
    }
};

enum directions_t {
    NW=0, N, NE,
    W,    X,  E,
    SW,   S, SE,
    NO_MATCHING_DIRECTION
};

directions_t direction_pressed(TCOD_key_t key)
{
    std::map<int, directions_t> spec_movemap; //Keypad, punctuation
    std::map<char, directions_t> char_movemap; //regular letters

    spec_movemap[TCODK_KP7] = directions_t::NW;
    spec_movemap[TCODK_KP8] = directions_t::N;
    spec_movemap[TCODK_KP9] = directions_t::NE;
    spec_movemap[TCODK_KP6] = directions_t::E;
    spec_movemap[TCODK_KP3] = directions_t::SE;
    spec_movemap[TCODK_KP2] = directions_t::S;
    spec_movemap[TCODK_KP1] = directions_t::SW;
    spec_movemap[TCODK_KP4] = directions_t::W;

    spec_movemap[TCODK_KP5] = directions_t::X;

    // char_movemap[TCODK_KP7] = directions_t::NW;
    char_movemap['n'] = directions_t::N;
    // char_movemap[TCODK_KP9] = directions_t::NE;
    char_movemap['e'] = directions_t::E;
    // char_movemap[TCODK_KP3] = directions_t::SE;
    char_movemap['s'] = directions_t::S;
    // char_movemap[TCODK_KP1] = directions_t::SW;
    char_movemap['w'] = directions_t::W;

    if (key.vk == TCODK_CHAR) 
    {
        auto it = char_movemap.find(key.c);
        if(it == char_movemap.end())
        {
            return directions_t::NO_MATCHING_DIRECTION;
        }
        return it->second;
    }
    else
    {
        auto it = spec_movemap.find(key.vk);
        if(it == spec_movemap.end())
        {
            return directions_t::NO_MATCHING_DIRECTION;
        }
        return it->second;
    }
    // return directions_t::N;
};

bool process_basic_cmd(TCOD_key_t request, Person *player)
{
    basic_cmds_t basic_cmd = basic_cmd_pressed(request);

    //ignore key releases
    if (request.pressed == false)
    {
        return false;
    };

    if (basic_cmd == basic_cmds_t::Pickup)
    {
        std::cout << "PICKUP THIS IS A STICKUP" << std::endl;

        //check if items are on the floor
        if (player->my_tile->check_for_items())
        {
            std::cout << "items on the floor, you'll be picking up";
            std::cout << "the last item you picked up now" << std::endl;
            //TODO:open ui for item pickup to choose which item
            Item* item = player->my_tile->inventory->items->back();
            player->pickUpItem(item);
            // player->equipment->chest->AddToSlot(item);

            return true;

        };
    }

    else if ( basic_cmd == basic_cmds_t::OpenInventory )
    {
        Game::current_state = GameStates::InventoryState;
        Game::current_screen = Screens::InventoryScreen;
    }

    else if ( basic_cmd == basic_cmds_t::OpenMagic )
    {
        Game::current_state = GameStates::MenuState;
        Game::current_screen = Screens::SpellSelectScreen;
    }

    else if ( basic_cmd == basic_cmds_t::OpenClassSelect )
    {
        Game::current_state = GameStates::MenuState;
        Game::current_screen = Screens::ClassSelectScreen;
    }

    else if ( basic_cmd == basic_cmds_t::OpenCharacterSheet )
    {
        Game::current_state = GameStates::MenuState;
        Game::current_screen = Screens::CharacterSheetScreen;
    }

    else if ( basic_cmd == basic_cmds_t::OpenHelp )
    {
        Game::current_state = GameStates::MenuState;
        Game::current_screen = Screens::HelpScreen;
    }

    else if ( basic_cmd == basic_cmds_t::ActivateDoor )
    {
        //determine the door to open
        //get the tile of the direction the player is facing
        Tile* door_tile;
        int* direction = Game::player->get_direction_heading();
        int x, y;

        x = player->x+direction[0];
        y = player->y+direction[1];

        door_tile = Game::world->getTileAt(x, y);

        //get the door that's on it
        if (door_tile->type_id == TileTypes::DoorTileTypeType) //TODO: make an enum or something for type_ids
        {
            std::cout << "there's a door here, its gon get open" << std::endl;
            // door_tile->map->l_map->setProperties(x, y, true, true);
            door_tile->ToggleDoor();
        }
        else 
        {
            std::cout << "there's NO DAMN door here" << std::endl;
        };
        //open the door
    }

    else if ( basic_cmd == basic_cmds_t::DownStairs )
    {
        Tile* stair_tile = Game::player->my_tile;
        if (stair_tile->type_id == TileTypes::StairsDownTileTypeType)
        {
            //std::cout << "there's a stair down here, its gon go down" << std::endl;
            new Message(Ui::msg_handler_main, NOTYPE_MSG,"There's a stair down here, its gon go down to the next floor.");
            ((StairsDownTileType*)stair_tile)->GoDown();
        }
        else
        {
            //std::cout << "There's no stair here" << std::endl;
            new Message(Ui::msg_handler_main, NOTYPE_MSG,"There's no stair here.");
        };
    }

    // else if ( basic_cmd == basic_cmds_t::OpenMagic )
    // {
    //     Tile* stair_tile = Game::player->my_tile;
    //     if (Ui::toggle_targetting())
    //     {
    //         new Message(Ui::msg_handler_main, NOTYPE_MSG, "Select a target.");
    //     }
    //     else
    //     {
    //         new Message(Ui::msg_handler_main, NOTYPE_MSG, "Done targetting");
    //     }
    //     //start targetting mode
    // }

    else if ( basic_cmd == basic_cmds_t::ConfirmCast )
    {
        Spell* spell = (Spell*)Ui::chosen_generic;
        if (spell == NULL)
        {
            std::cout << "no spell chosen" << std::endl;
            return false;
        }
        Tile* stair_tile = Game::player->my_tile;
        Tile* mouse_tile = Game::get_mouse_tile();
        // Spell* spell = Game::player->spells->back();
        int mana_cost = spell->mana_cost;
        int spell_range = spell->max_range;
        int spell_damage = spell->spell_effect->health_current_val;
        // int spell_damage = (-spell->attrs->health->current_val);

        int distance = get_euclidean_distance(Game::player->x, Game::player->y, mouse_tile->tile_x, mouse_tile->tile_y);
        if (Ui::is_targetting && mouse_tile->is_occupied())
        {
            if (distance <= spell_range)
            {
                if (Game::player->attrs->mana->current_val > mana_cost)
                {
                    mouse_tile->occupant->combat->TakeDamage(Game::player->combat, spell_damage);
                    Game::player->attrs->mana->current_val -= mana_cost;
                    new Message(Ui::msg_handler_main, NOTYPE_MSG, "BAM casted a spell at the range of %i", distance, ".");
                    return true;
                }
                else 
                {
                    new Message(Ui::msg_handler_main, NOTYPE_MSG, "No mana for this cast!");
                };
            }
            else
            {
                new Message(Ui::msg_handler_main, NOTYPE_MSG, "Out of range. Max is %i, you're at %i.", spell_range, distance);
            };
        }
        else
        {
            new Message(Ui::msg_handler_main, NOTYPE_MSG, "Pick an actual target how about.");
        };
        //start targetting mode

        return false;
    };
};

bool process_inventory_item_active(TCOD_key_t request, Person *player)
{
    inventory_items_active_t action = inventory_items_active_pressed(request);

    if( action == inventory_items_active_t::ExamineItem )
    {
        new Message(Ui::msg_handler_main, NOTYPE_MSG, "%s", Ui::chosen_item->description);
        std::cout << Ui::chosen_item->description << std::endl;
        return true;
    }
    else if( action == inventory_items_active_t::DropItem )
    {
        new Message(Ui::msg_handler_main, NOTYPE_MSG, "DROP ITEM.");
        Item* item = Ui::chosen_item;
        Ui::chosen_item = NULL;
        Ui::item_active = false;

        player->inventory->drop_item(item);
        return true;
    }

    else if( action == inventory_items_active_t::UseItem )
    {
        new Message(Ui::msg_handler_main, NOTYPE_MSG, "Using item.");
        Ui::chosen_item->use(Game::player);
        return true;
    }

    else if( action == inventory_items_active_t::EquipItem )
    {
        Ui::chosen_item->equip(Game::player);
        Game::player->equipment->equip_item(Ui::chosen_item);
        new Message(Ui::msg_handler_main, NOTYPE_MSG, "Equipping item.");
        return true;
    }

    else if( action == inventory_items_active_t::UnequipItem )
    {
        Ui::chosen_item->unequip(Game::player);
        Game::player->equipment->unequip_item(Ui::chosen_item);
        new Message(Ui::msg_handler_main, NOTYPE_MSG, "Unequipping item.");
        return true;
    }

    else if( action == inventory_items_active_t::EscapeMenuItem )
    {
        Ui::item_active = false;
        Ui::chosen_item = false;
        new Message(Ui::msg_handler_main, NOTYPE_MSG, "Escape back to regular inventory mode.");
        return true;
    }

    return false;
};

bool process_classes_active(TCOD_key_t request, Person *player)
{
    classes_active_t action = classes_active_pressed(request);

    if( action == classes_active_t::ExamineClass )
    {
        new Message(Ui::msg_handler_main, NOTYPE_MSG, "EXAMINE CLASS.");
        std::cout << "EXAMINE CLASS." << std::endl;
        return true;
    }
    else if( action == classes_active_t::DropClass )
    {/*
        new Message(Ui::msg_handler_main, NOTYPE_MSG, "DROP ITEM.");
        Class* spell = Ui::chosen_item;
        Ui::chosen_spell = NULL;
        Ui::spell_active = false;

        player->inventory->drop_spell(item);
        return true;
        */}

    else if( action == classes_active_t::CastClass )
    {
        // Ui::toggle_targetting();
        // Game::current_state = GameStates::GameplayState;
        // std::cout << Ui::chosen_spell->name << std::endl;
        // Game::current_screen = Screens::Game
        //new Message(Ui::msg_handler_main, NOTYPE_MSG, "Using spell.");
        //Ui::chosen_spell->use(Game::player);
        //return true;
    }

    else if( action == classes_active_t::EquipClass )
    {
        Game::player->actor_class = Ui::chosen_class;
        std::cout << "Class changed to " << Ui::chosen_class->name << std::endl;
        // Ui::chosen_spell->equip(Game::player);
        // Game::player->equipment->equip_spell(Ui::chosen_item);
        // new Message(Ui::msg_handler_main, NOTYPE_MSG, "Equipping spell.");
        // return true;
    }

    else if( action == classes_active_t::UnequipClass )
    {
        // Ui::chosen_spell->unequip(Game::player);
        // Game::player->equipment->unequip_spell(Ui::chosen_item);
        // new Message(Ui::msg_handler_main, NOTYPE_MSG, "Unequipping spell.");
        // return true;
    }

    else if( action == classes_active_t::EscapeMenuClass )
    {
        std::cout << "escape" << std::endl;
        // if (  Ui::class_active == false )
        // {
        //     Game::current_state = GameStates::GameplayState;
        // };
        Ui::generic_active = false;
        Ui::chosen_generic = false;
        new Message(Ui::msg_handler_main, NOTYPE_MSG, "Escape back to regular inventory mode.");
        std::cout << "Escape back to regular classmenu mode." << std::endl;
        return true;
    }

    return false;
};

bool process_spells_active(TCOD_key_t request, Person *player)
{
    spells_active_t action = spells_active_pressed(request);

    if( action == spells_active_t::ExamineSpell )
    {
        new Message(Ui::msg_handler_main, NOTYPE_MSG, "EXAMINE SPELL.");
        std::cout << "EXAMINE SPELL." << std::endl;
        return true;
    }
    else if( action == spells_active_t::DropSpell )
    {/*
        new Message(Ui::msg_handler_main, NOTYPE_MSG, "DROP ITEM.");
        Spell* spell = Ui::chosen_item;
        Ui::chosen_spell = NULL;
        Ui::spell_active = false;

        player->inventory->drop_spell(item);
        return true;
        */}

    else if( action == spells_active_t::CastSpell )
    {
        Ui::toggle_targetting();
        Game::current_state = GameStates::GameplayState;
        std::cout << ((Spell*)Ui::chosen_generic)->name << std::endl;
        // Game::current_screen = Screens::Game
        //new Message(Ui::msg_handler_main, NOTYPE_MSG, "Using spell.");
        //Ui::chosen_spell->use(Game::player);
        //return true;
    }

    else if( action == spells_active_t::EquipSpell )
    {
        // Ui::chosen_spell->equip(Game::player);
        // Game::player->equipment->equip_spell(Ui::chosen_item);
        // new Message(Ui::msg_handler_main, NOTYPE_MSG, "Equipping spell.");
        // return true;
    }

    else if( action == spells_active_t::UnequipSpell )
    {
        // Ui::chosen_spell->unequip(Game::player);
        // Game::player->equipment->unequip_spell(Ui::chosen_item);
        // new Message(Ui::msg_handler_main, NOTYPE_MSG, "Unequipping spell.");
        // return true;
    }

    else if( action == spells_active_t::EscapeMenuSpell )
    {
        Ui::generic_active = false;
        Ui::chosen_generic = false;
        new Message(Ui::msg_handler_main, NOTYPE_MSG, "Escape back to regular spellmenu mode.");
        std::cout <<  "Escape back to regular spellmenu mode." << std::endl;
        return true;
    }

    return false;
};

void move_camera(int dir_x, int dir_y)
{
    //if player moves within 5 spaces of the edge of the map, scroll the camera 
    //so that they're no longer within 5 spaces

    // camera checks player position relative to camera
    // then checks camera position relative to world
    // if player 5< spaces from cam limits move camera so that player has 5>
    // spaces assuming camera has that much space
    int cam_x2 = Game::camera_x + Game::camera_w;
    int cam_y2 = Game::camera_y + Game::camera_h;

    //relative player pos
    int plr_x = Game::player->x - Game::camera_x;
    int plr_y = Game::player->y - Game::camera_y;

    int border_threshold = 10;
    //std::cout << "cam w" << Game::camera_w << std::endl;

    int change = 1;
    if (plr_x <= border_threshold || plr_x >= (Game::camera_w -border_threshold))
    {
        //adjust camera horizontally
        if (plr_x <= border_threshold) 
        {
            if (Game::camera_x - change >= (0))
            {
                Game::camera_x -= 1;
                // std::cout << "x left" << std::endl;
            }
            else
            {
                // std::cout << "x NOT left" << std::endl;
            }
        }
        else 
        {
            if (cam_x2 + change <= (Game::map_width))
            {
                Game::camera_x += 1;
                // std::cout << "x right" << std::endl;
            }
            else
            {
                // std::cout << "x NOT right" << std::endl;
            }
        };
    };
    if (plr_y <= border_threshold || plr_y >= (Game::camera_h -border_threshold))
    {
        //adjust camera vertically
        if (plr_y <= border_threshold) 
        {
            if (Game::camera_y - change >= (0 ))
            {
                Game::camera_y -= 1;
                // std::cout << "y up" << std::endl;
            }
            else
            {
                // std::cout << "y NOT up" << std::endl;
            }
        }
        else 
        {
            if (cam_y2 + change <= (Game::map_height))
            {
                Game::camera_y += 1;
                // std::cout << "y down" << std::endl;
            }
            else
            {
                // std::cout << "y NOT down" << std::endl;
            }
        };
    };
};

//returns whether or not the player has moved and should increment the turn
bool process_movement(TCOD_key_t request, Person *player)
{
    Map *world = Game::world;
    bool buildmode = Game::buildmode;

    int orig_plr_pos_x, orig_plr_pos_y;
    orig_plr_pos_x = player->x;
    orig_plr_pos_y = player->y;

    player->is_moving_up = false;
    player->is_moving_down = false;
    player->is_moving_left = false;
    player->is_moving_right = false;
    //reset player facing directions
    if (request.pressed == false)
    {
        player->resetIsMoving();
        return false;
    };

    directions_t direction = direction_pressed(request);
    if( direction == directions_t::N )
    {
        player->is_moving_up = true;
        if(Game::current_map->attackMovePlayer(player, 0, -1) )
        { 
            move_camera(0, -1);
            return true;
        }
    }
    else if( direction == directions_t::NE )
    {
        player->is_moving_up = true;
        player->is_moving_right = true;
        if(Game::current_map->attackMovePlayer(player, 1, -1) )
        { 
            move_camera(1, -1);
            return true;
        }
    }
    else if( direction == directions_t::S )
    {
        player->is_moving_down = true;
        if(Game::current_map->attackMovePlayer(player, 0, 1) )
        { 
            move_camera(0, 1);
            return true;
        }
    }
    else if( direction == directions_t::SE)
    {
        player->is_moving_down = true;
        player->is_moving_right = true;
        if(Game::current_map->attackMovePlayer(player, 1, 1) )
        {
            move_camera(1, 1);
            return true;
        }
    }
    else if( direction == directions_t::E)
    {
        player->is_moving_right = true;
        if(Game::current_map->attackMovePlayer(player, 1, 0) )
        {
            move_camera(1, 0);

            return true;
        }
    }
    else if( direction == directions_t::SW)
    {
        player->is_moving_left = true;
        player->is_moving_down = true;
        if(Game::current_map->attackMovePlayer(player, -1, 1) )
        { 
            move_camera(-1, 1);

            return true;
        }
    }
    else if( direction == directions_t::NW)
    {
        player->is_moving_left = true;
        player->is_moving_up = true;
        if(Game::current_map->attackMovePlayer(player, -1, -1) )
        { 
            move_camera(-1, -1);

            return true;
        }
    }
    else if( direction == directions_t::W)
    {
        player->is_moving_left = true;
        if(Game::current_map->attackMovePlayer(player, -1, 0) )
        { 
            move_camera(-1, 0);

            return true;
        }
    }
    else if( direction == directions_t::X)
    {
        std::cout << "WAITING" << std::endl;
        // player->is_moving_left = true;
        // if(Game::current_map->attackMovePlayer(player, -1, 0) )
        // { 
        return true;
        // }
    };


    //if the player has moved or attacked this update, increment the turn
    //counter
    if ((orig_plr_pos_x != player->x || orig_plr_pos_y != player->y || player->has_attacked))
    {
        return true;
    }
    else
    {
        return false;
    }

};

bool is_request_basic_cmd(TCOD_key_t request)
{
    return basic_cmd_pressed(request) != basic_cmds_t::NO_MATCHING_BASIC_CMD;
};

bool is_request_move_cmd(TCOD_key_t request)
{
    return direction_pressed(request) != directions_t::NO_MATCHING_DIRECTION;
};

bool is_request_inventory_item_active_cmd(TCOD_key_t request)
{
    return inventory_items_active_pressed(request) != inventory_items_active_t::NO_MATCHING_ITEMS_ACTIVE;
};

bool is_request_spell_active_cmd(TCOD_key_t request)
{
    return spells_active_pressed(request) != spells_active_t::NO_MATCHING_SPELLS_ACTIVE;
};

bool is_request_class_active_cmd(TCOD_key_t request)
{
    return classes_active_pressed(request) != classes_active_t::NO_MATCHING_CLASSES_ACTIVE;
};

void process_buildmode(TCOD_key_t request, int current_tile)
{
    // Map *world = Game::world;
    // bool buildmode = Game::buildmode;
    // if(request.c == 'c')
    // {
    //     // do the stuff to make a new tile
    //     std::cout << "type_id: ";
    //     Tile this_tile = Game::current_map->tileArray[current_tile];
    //     std::cin >> this_tile.type_id;
    //     if(this_tile.type_id == TileTypes::WarpTileTypeType)
    //     {
    //         WarpTileType* warp_tile;
    //         warp_tile = (WarpTileType*) this_tile.tile;

    //         std::cout << "Warp Map: ";
    //         std::cin >> warp_tile->warpMap;
    //         std::cout << "WarpX: ";
    //         std::cin >> warp_tile->warpX;
    //         std::cout << "WarpY: ";
    //         std::cin >> warp_tile->warpY;
    //     }
    //     std::cout << std::endl << "Description: ";
    //     getline(std::cin, this_tile.tile->description);  // do this twice because hitting enter... whatever
    //     getline(std::cin, this_tile.tile->description);
    //     std::cout << std::endl << "Representation: ";
    //     std::cin >> this_tile.tile->representation->repr;
    // }
    // else if(request.c == 'i')
    // {
    //     // Write the map to a file
    //     std::ofstream myfile;
    //     std::string filename;
    //     std::cout << "Filename: ";
    //     getline(std::cin, filename); // do this twice because hitting enter... whatever
    //     myfile.open (filename);
    //     int i,j;

    //     // Map this_map = Game::current_map;
    //     myfile << Game::current_map->width << std::endl;
    //     myfile << Game::current_map->height << std::endl;  
    //     myfile << Game::current_map->description << std::endl;  

    //     for(i=0; i<Game::current_map->height; i++)
    //         for(j=0; j<Game::current_map->width; j++)
    //         {
    //             Tile active_tile = Game::current_map->tileArray[(i*Game::current_map->width)+j];
    //             myfile << active_tile.tile->representation << std::endl;
    //             myfile << active_tile.type_id << std::endl;
    //             if(active_tile.type_id == TileTypes::WarpTileTypeType)
    //             {
    //                 WarpTileType* warp_tile;
    //                 warp_tile = (WarpTileType*) active_tile.tile;

    //                 myfile << warp_tile->warpMap << std::endl;
    //                 myfile << warp_tile->warpX << std::endl;
    //                 myfile << warp_tile->warpY << std::endl;
    //             }
    //             myfile << active_tile.tile->description << std::endl;  
    //         }
    //     myfile.close();
    // }

    // else if(request.c == 'o')
    // {
    //     // Copy a tile
    //     Game::clipboard = &Game::current_map->tileArray[current_tile];
    // }

    // else if(request.c == 'p')
    // {
    //     // paste a tile
    //     Game::current_map->tileArray[current_tile] = *Game::clipboard;
    // }

};

void process_mouse_inv_event()
{
    if (Game::mouse_evt.lbutton_pressed)
    {

    };
};

bool process_mouse_event(TCOD_mouse_t request, Person *player)
{

    if (request.dx > Ui::mouse_move_threshold || request.dy > Ui::mouse_move_threshold)
    {
        //std::cout << "Mouse MOVE" << std::endl;
        Ui::tick_mouse_moved = Game::tick_count;
    }
    //set the foreground color to red for the tile the mouse is on
    // Tile* moused_tile = Game::current_map->getTileAt(Game::mouse_evt.cx+Game::camera_x, Game::mouse_evt.cy+Game::camera_y);
    // moused_tile->tile->representation->temp_bg_color = &(TCODColor)(TCODColor::red); //this only works because we get a new red every turn

    if (request.lbutton_pressed)
    {
        std::cout << "mouse lclicked" << std::endl;
    }

    return 0;
};

bool process_debug_event(TCOD_key_t request, Person *player)
{
    if (request.vk == TCODK_F2)
    {
        exit(1);
    };

    if (request.vk == TCODK_F3)
    {
        Game::debug_opts->all_vision = !Game::debug_opts->all_vision;
    }

    if (request.vk == TCODK_F4)
    {
        auto map = Game::build_world();
        Game::current_map = map;
    }

    if (request.vk == TCODK_F5)
    {
        //this'll redraw the entire screen incase shit goes black
        TCODConsole::root->setDirty(0, 0, 1000, 1000);
    }

    if (request.vk == TCODK_F6)
    {
        //this'll redraw the entire screen incase shit goes black
        TCODConsole::root->setDirty(0, 0, 1000, 1000);
    }

    if (request.vk == TCODK_F7)
    {
        //this'll redraw the entire screen incase shit goes black
        Game::player->combat->LevelUp(1);
    }

    if (request.vk == TCODK_F8)
    {
        //this'll redraw the entire screen incase shit goes black
        TCODConsole::root->setDirty(0, 0, 1000, 1000);
    }
};

bool process_key_event(TCOD_key_t request, Person *player)
{

    Ui::tick_key_pressed = Game::tick_count;
    //std::cout << "key event" << std::endl;
    //determine if movement command
    bool incr_turn = false;
    int current_tile = player->x+(player->y*(Game::current_map->width));

    switch(Game::current_state)
    {
        case GameStates::GameplayState: 

            if(is_request_move_cmd(request))
            {
                incr_turn = process_movement(request, player);
                if (incr_turn)
                {
                    //stop the targetting so that user has to retry
                    Ui::is_targetting = false;
                    Ui::chosen_spell = NULL;
                    Ui::generic_active = false;
                }
            }

            else if (is_request_basic_cmd(request))
            {
                incr_turn = process_basic_cmd(request, player);
            }

            else if(request.vk == TCODK_ESCAPE && request.pressed == 1)
            {
                std::cout << "Goodbye now" << std::endl;
                exit(1);
            }
            else
            {
                std::cout << std::endl << "command not found: " << char_to_str(request.c) << std::endl;
                std::cout << "nswe or numpad to move, i to open inventory, q to quit, o to open doors" << std::endl;
            }

            break;

        case GameStates::MenuState:
            if (Game::current_screen == Screens::SpellSelectScreen)
            {

                std::vector<Spell*>* spells = Game::player->spells;
                int size = spells->size();
                generic_keypair_t spell_map = build_keypairs(size);

                bool successful_action = true;
                if (Ui::generic_active == false)
                {
                    if (request.c == 'q' && request.pressed == 1 )
                    {
                        std::cout << "Back to the game. spell" << std::endl;
                        Ui::chosen_generic = NULL;
                        Ui::generic_active = false;
                        Game::current_state = GameStates::GameplayState;
                    }
                    //choose spell
                    generic_keypair_t::iterator it = spell_map.find(request.c);
                    if (it != spell_map.end())
                    {
                        if ((Spell*)Ui::chosen_generic == spells->at(it->second))
                        {
                            Ui::generic_active = true;
                        }
                        else
                        {
                            Ui::generic_active = false;
                        };
                        Ui::chosen_generic = spells->at(it->second);
                    };
                }
                else // generic_active is true
                {
                    if (is_request_spell_active_cmd(request))
                    {
                        successful_action = process_spells_active(request, player);
                    }
                    else 
                    {
                        std::cout << std::endl << "command not found: " << char_to_str(request.c) << std::endl;
                        std::cout << "q to return to gameplay, a b c to choose the first, second, third spell etc." << std::endl;
                        std::cout << "press again to select. once it's activated, press u to use" << std::endl;
                        std::cout << "e to equip, y to unequip, d to drop" << std::endl;
                    }
                }

            }
            else if (Game::current_screen == Screens::ClassSelectScreen)
            {

                std::vector<IClass*>* classes = Actor::actor_class_choices;

                select_generic(request, classes, is_request_class_active_cmd, process_classes_active);

            };
            break;

        case GameStates::InventoryState:
            //generate keys for the appropriate items
            typedef std::unordered_map<char, Item*> keypair_t;
            keypair_t item_map;
            typedef std::pair<char, Item*> keypair;

            char key = 'a';

            std::vector<Item*>* items = Game::player->inventory->items;
            for (std::vector<Item*>::const_iterator it = items->begin(); it != items->end(); ++it)
            {
                item_map.insert(keypair(key, (*it)));
                key++;
            };

            bool successful_action = true;
            if (Ui::item_active == false)
            {
                if (request.c == 'q' && request.pressed == 1 && Ui::generic_active == false && Ui::item_active == false && Ui::item_active == false)
                {
                    std::cout << "Back to the game. invent" << std::endl;
                    Ui::chosen_item = NULL;
                    Ui::item_active = false;
                    Game::current_state = GameStates::GameplayState;
                }
                //choose item
                auto it = item_map.find(request.c);
                if (it != item_map.end())
                {
                    if (Ui::chosen_item == it->second)
                    {
                        Ui::item_active = true;
                    }
                    else
                    {
                        Ui::item_active = false;
                    };
                    Ui::chosen_item = it->second;
                };
            }
            else // item_active is true
            {
                if (is_request_inventory_item_active_cmd(request))
                {
                    successful_action = process_inventory_item_active(request, player);
                }
                else 
                {
                    std::cout << std::endl << "command not found: " << char_to_str(request.c) << std::endl;
                    std::cout << "q to return to gameplay, a b c to choose the first, second, third item etc." << std::endl;
                    std::cout << "press again to select. once it's activated, press u to use" << std::endl;
                    std::cout << "e to equip, y to unequip, d to drop" << std::endl;
                }
            }

            break;
    }

    return incr_turn;

};



// generic_keypair_t build_keypairs(std::vector<int> indices)
generic_keypair_t build_keypairs(int limit)
{
    generic_keypair_t keymap;
    char key = 'a';
    // for (std::vector<int>::iterator it = indices.begin(); it != indices.end(); it++)
    for (int index = 0; index < limit; index++)
    {
        keymap.insert(generic_keypair(key, index));
        if (key == 'z') break;
        key++;
    }

    return keymap;
};

    template<class T>
void select_generic(TCOD_key_t request, std::vector<T*>* generic_vector, bool (*active_func)(TCOD_key_t), bool (*process_func)(TCOD_key_t, Person*))
{
    int size = generic_vector->size();
    generic_keypair_t class_map = build_keypairs(size);
    generic_keypair_t::iterator it = class_map.find(request.c);
    bool successful_action = true;
    if (Ui::generic_active == false)
    {
        if (request.c == 'q' && request.pressed == 1 && Ui::generic_active == false && Ui::generic_active == false && Ui::generic_active == false)
        {
            std::cout << "Back to the game. class" << std::endl;
            Ui::chosen_generic = NULL;
            Ui::generic_active = false;
            Game::current_state = GameStates::GameplayState;
        }
        //choose class
        generic_keypair_t::iterator it = class_map.find(request.c);
        if (it != class_map.end())
        {
            if ((IClass*)Ui::chosen_generic == generic_vector->at(it->second))
            {
                Ui::generic_active = true;
            }
            else
            {
                Ui::generic_active = false;
            };
            Ui::chosen_generic = generic_vector->at(it->second);
        };
    }
    else // generic_active is true
    {
        if (active_func(request))
        {
            successful_action = process_func(request, Game::player);
        }
        else 
        {
            std::cout << std::endl << "command not found: " << char_to_str(request.c) << std::endl;
            std::cout << "q to return to gameplay, a b c to choose the first, second, third class etc." << std::endl;
            std::cout << "press again to select. once it's activated, press u to use" << std::endl;
            std::cout << "e to equip, y to unequip, d to drop" << std::endl;
        }
    }

};
