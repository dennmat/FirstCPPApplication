#include "stdafx.h"
// #include "libtcod.hpp"
#include <iostream>
#include <sstream>
#include <fstream>
#include <typeinfo>
#include <cstdlib>
#include <algorithm>
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

using namespace std;

enum basic_cmds_t {
    Pickup, Drop,
    OpenInventory,
    Look, ActivateDoor,
    NO_MATCHING_BASIC_CMD
};

basic_cmds_t  basic_cmd_pressed(TCOD_key_t key)
{
    std::map<int, basic_cmds_t> spec_movemap; //Keypad, punctuation
    std::map<char, basic_cmds_t> char_movemap; //regular letters

    // spec_movemap[','] = basic_cmds_t::Pickup;
    // spec_movemap[TCODK_KP8] = 

    char_movemap['i'] = basic_cmds_t::OpenInventory;
    char_movemap[','] = basic_cmds_t::Pickup;
    char_movemap['o'] = basic_cmds_t::ActivateDoor;

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
        cout << "PICKUP THIS IS A STICKUP" << endl;

        //check if items are on the floor
        if (player->my_tile->check_for_items())
        {
            cout << "items on the floor, you'll be picking up";
            cout << "the last item you picked up now" << endl;
            //TODO:open ui for item pickup to choose which item
            Item* item = player->my_tile->inventory->items->back();
            player->pickUpItem(item);
            // player->equipment->chest->AddToSlot(item);

            return true;

        };
    }

    else if ( basic_cmd == basic_cmds_t::OpenInventory )
    {
        Game::current_state = GameStates::MenuState;
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
        if (door_tile->type_id == 4) //TODO: make an enum or something for type_ids
        {
            std::cout << "there's a door here, its gon get open" << std::endl;
            // door_tile->map->l_map->setProperties(x, y, true, true);
            ((DoorTileType*)door_tile->tile)->ToggleDoor();
        }
        else 
        {
            std::cout << "there's NO DAMN door here" << std::endl;
        };
        //open the door
    };

    return false;
};

bool process_inventory_item_active(TCOD_key_t request, Person *player)
{
    inventory_items_active_t action = inventory_items_active_pressed(request);

    if( action == inventory_items_active_t::ExamineItem )
    {
        std::cout << "EXAMINE ITEM" << std::endl;
        return true;
    }
    else if( action == inventory_items_active_t::DropItem )
    {
        std::cout << "DROP ITEM" << std::endl;
        Item* item = Ui::chosen_item;
        Ui::chosen_item = NULL;
        Ui::item_active = false;

        player->inventory->drop_item(item);
        return true;
    }

    else if( action == inventory_items_active_t::UseItem )
    {
        std::cout << "Using item" << std::endl;
        Ui::chosen_item->use(Game::player);
        return true;
    }

    else if( action == inventory_items_active_t::EquipItem )
    {
        Ui::chosen_item->equip(Game::player);
        Game::player->equipment->equip_item(Ui::chosen_item);
        std::cout << "Equipping item" << std::endl;
        return true;
    }

    else if( action == inventory_items_active_t::UnequipItem )
    {
        Ui::chosen_item->unequip(Game::player);
        Game::player->equipment->unequip_item(Ui::chosen_item);
        std::cout << "Unequipping item" << std::endl;
        return true;
    }

    else if( action == inventory_items_active_t::EscapeMenuItem )
    {
        Ui::item_active = false;
        Ui::chosen_item = false;
        std::cout << "Escape back to regular inventory mode" << std::endl;
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

    int border_threshold = 5;
    std::cout << "cam w" << Game::camera_w << std::endl;

    int change = 1;
    if (plr_x <= border_threshold || plr_x >= (Game::camera_w -border_threshold))
    {
        //adjust camera horizontally
        if (plr_x <= border_threshold) 
        {
            if (Game::camera_x - change >= (0))
            {
                Game::camera_x -= 1;
                std::cout << "x left" << std::endl;
            }
            else
            {
                std::cout << "x NOT left" << std::endl;
            }
        }
        else 
        {
            if (cam_x2 + change <= (Game::map_width))
            {
                Game::camera_x += 1;
                std::cout << "x right" << std::endl;
            }
            else
            {
                std::cout << "x NOT right" << std::endl;
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
                std::cout << "y up" << std::endl;
            }
            else
            {
                std::cout << "y NOT up" << std::endl;
            }
        }
        else 
        {
            if (cam_y2 + change <= (Game::map_height))
            {
                Game::camera_y += 1;
                std::cout << "y down" << std::endl;
            }
            else
            {
                std::cout << "y NOT down" << std::endl;
            }
        };
    };
};

//returns whether or not the player has moved
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
        cout << "WAITING" << endl;
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

void process_buildmode(TCOD_key_t request, int current_tile)
{
    Map *world = Game::world;
    bool buildmode = Game::buildmode;
    if(request.c == 'c')
    {
        // do the stuff to make a new tile
        cout << "type_id: ";
        Tile this_tile = Game::current_map->tileArray[current_tile];
        cin >> this_tile.type_id;
        if(this_tile.type_id == 2)
        {
            WarpTileType* warp_tile;
            warp_tile = (WarpTileType*) this_tile.tile;

            cout << "Warp Map: ";
            cin >> warp_tile->warpMap;
            cout << "WarpX: ";
            cin >> warp_tile->warpX;
            cout << "WarpY: ";
            cin >> warp_tile->warpY;
        }
        cout << endl << "Description: ";
        getline(cin, this_tile.tile->description);  // do this twice because hitting enter... whatever
        getline(cin, this_tile.tile->description);
        cout << endl << "Representation: ";
        cin >> this_tile.tile->representation->repr;
    }
    else if(request.c == 'i')
    {
        // Write the map to a file
        ofstream myfile;
        string filename;
        cout << "Filename: ";
        getline(cin, filename); // do this twice because hitting enter... whatever
        myfile.open (filename);
        int i,j;

        // Map this_map = Game::current_map;
        myfile << Game::current_map->width << endl;
        myfile << Game::current_map->height << endl;  
        myfile << Game::current_map->description << endl;  

        for(i=0; i<Game::current_map->height; i++)
            for(j=0; j<Game::current_map->width; j++)
            {
                Tile active_tile = Game::current_map->tileArray[(i*Game::current_map->width)+j];
                myfile << active_tile.tile->representation << endl;
                myfile << active_tile.type_id << endl;
                if(active_tile.type_id == 2)
                {
                    WarpTileType* warp_tile;
                    warp_tile = (WarpTileType*) active_tile.tile;

                    myfile << warp_tile->warpMap << endl;
                    myfile << warp_tile->warpX << endl;
                    myfile << warp_tile->warpY << endl;
                }
                myfile << active_tile.tile->description << endl;  
            }
        myfile.close();
    }

    else if(request.c == 'o')
    {
        // Copy a tile
        Game::clipboard = &Game::current_map->tileArray[current_tile];
    }

    else if(request.c == 'p')
    {
        // paste a tile
        Game::current_map->tileArray[current_tile] = *Game::clipboard;
    }

};

bool process_mouse_event(TCOD_mouse_t request, Person *player)
{

    //set the foreground color to red for the tile the mouse is on
    Tile* moused_tile = Game::current_map->getTileAt(Game::mouse_evt.cx, Game::mouse_evt.cy);
    moused_tile->tile->representation->temp_bg_color = &(TCODColor)(TCODColor::red); //this only works because we get a new red every turn

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
};

bool process_key_event(TCOD_key_t request, Person *player)
{

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
                    int item_count = player->my_tile->inventory->get_count();
                    if (item_count > 0)
                    {
                        std::string msg_str = (item_count == 1) ? "An item is on the ground" : "%d items are on the ground";
                        Message* msg = new Message(Ui::msg_handler_main, msg_str, item_count);
                    }
                    else 
                    {
                        Message* msg = new Message(Ui::msg_handler_main, "Nothing on the ground");
                    }
                        Message* msg = new Message(Ui::msg_handler_main, "%s", player->my_tile->tile->description.c_str());
                };
            }

            else if (is_request_basic_cmd(request))
            {
                incr_turn = process_basic_cmd(request, player);
            }

            else if(request.c == 'q' && request.pressed == 1)
            {
                cout << "Goodbye now" << endl;
                exit(1);
            }
            else
            {
                cout << endl << "command not found: " << char_to_str(request.c) << endl;
                cout << "nswe or numpad to move, i to open inventory, q to quit, o to open doors" << endl;
            }

            break;

        case GameStates::MenuState:
            if (request.c == 'q' && request.pressed == 1 && Ui::item_active == false)
            {
                cout << "Back to the game." << endl;
                Ui::chosen_item = NULL;
                Ui::item_active = false;
                Game::current_state = GameStates::GameplayState;
            }

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
                    cout << endl << "command not found: " << char_to_str(request.c) << endl;
                    cout << "q to return to gameplay, a b c to choose the first, second, third item etc." << endl;
                    cout << "press again to select. once it's activated, press u to use" << endl;
                    cout << "e to equip, y to unequip, d to drop" << endl;
                }
            }

            //display info of chosen item

            //equip chosen item

            break;
    }

    return incr_turn;

};
