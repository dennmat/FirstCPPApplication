#include "stdafx.h"
// #include "libtcod.hpp"
#include <iostream>
#include <sstream>
#include <fstream>
#include <typeinfo>
#include <cstdlib>
#include <algorithm>

#include <windows.h>

#include "input.h"
#include "game.h"
#include "Person.h"
#include "map.h"
#include "utils.h"
#include "inventory.h"
#include "Representation.h"
#include "tile.h"	
#include "equipment.h"

using namespace std;

enum basic_cmds_t {
    Pickup, Drop,
    OpenInventory,
    Look,
    NO_MATCHING_BASIC_CMD
};

basic_cmds_t  basic_cmd_pressed(TCOD_key_t key)
{
    std::map<int, basic_cmds_t> spec_movemap; //Keypad, punctuation
    std::map<char, basic_cmds_t> char_movemap; //regular letters

    // spec_movemap[','] = basic_cmds_t::Pickup;
    // spec_movemap[TCODK_KP8] = 
    // spec_movemap[TCODK_KP9] = 
    // spec_movemap[TCODK_KP6] = 
    // spec_movemap[TCODK_KP3] = 
    // spec_movemap[TCODK_KP2] = 
    // spec_movemap[TCODK_KP1] = 
    // spec_movemap[TCODK_KP4] = 

    // char_movemap[TCODK_KP7] = basic_cmds_t::NW;
    //char_movemap['n'] = basic_cmds_t::N;
    char_movemap[','] = basic_cmds_t::Pickup;

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
    // return basic_cmds_t::N;

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

bool process_basic_cmd(Game* the_game, TCOD_key_t request, Person *player)
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
            player->equipment->chest->AddToSlot(item);

            return true;

        };
    };

    return false;
};

//returns whether or not the player has moved
bool process_movement(Game* the_game, TCOD_key_t request, Person *player)
{
    Map *world = the_game->world;
    bool buildmode = the_game->buildmode;

    int orig_plr_pos_x, orig_plr_pos_y;
    orig_plr_pos_x = player->x;
    orig_plr_pos_y = player->y;

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
        if(the_game->current_map->attackMovePlayer(player, 0, -1) )
        { 
            return true;
        }
    }
    else if( direction == directions_t::NE )
    {
        player->is_moving_up = true;
        player->is_moving_right = true;
        if(the_game->current_map->attackMovePlayer(player, 1, -1) )
        { 
            return true;
        }
    }
    else if( direction == directions_t::S )
    {
        player->is_moving_down = true;
        if(the_game->current_map->attackMovePlayer(player, 0, 1) )
        { 
            return true;
        }
    }
    else if( direction == directions_t::SE)
    {
        player->is_moving_right = true;
        player->is_moving_down = true;
        if(the_game->current_map->attackMovePlayer(player, 1, 1) )
        {
            return true;
        }
    }
    else if( direction == directions_t::E)
    {
        player->is_moving_right = true;
        if(the_game->current_map->attackMovePlayer(player, 1, 0) )
        {
            return true;
        }
    }
    else if( direction == directions_t::SW)
    {
        player->is_moving_left = true;
        player->is_moving_down = true;
        if(the_game->current_map->attackMovePlayer(player, -1, 1) )
        { 
            return true;
        }
    }
    else if( direction == directions_t::NW)
    {
        player->is_moving_left = true;
        player->is_moving_up = true;
        if(the_game->current_map->attackMovePlayer(player, -1, -1) )
        { 
            return true;
        }
    }
    else if( direction == directions_t::W)
    {
        player->is_moving_left = true;
        if(the_game->current_map->attackMovePlayer(player, -1, 0) )
        { 
            return true;
        }
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

void process_buildmode(Game* the_game, TCOD_key_t request, int current_tile)
{
    Map *world = the_game->world;
    bool buildmode = the_game->buildmode;
    if(request.c == 'c')
    {
        // do the stuff to make a new tile
        cout << "tiletype: ";
        Tile this_tile = the_game->current_map->tileArray[current_tile];
        cin >> this_tile.tiletype;
        if(this_tile.tiletype == 2)
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

        // Map this_map = the_game->current_map;
        myfile << the_game->current_map->width << endl;
        myfile << the_game->current_map->height << endl;  
        myfile << the_game->current_map->description << endl;  

        for(i=0; i<the_game->current_map->height; i++)
            for(j=0; j<the_game->current_map->width; j++)
            {
                Tile active_tile = the_game->current_map->tileArray[(i*the_game->current_map->width)+j];
                myfile << active_tile.tile->representation << endl;
                myfile << active_tile.tiletype << endl;
                if(active_tile.tiletype == 2)
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
        the_game->clipboard = &the_game->current_map->tileArray[current_tile];
    }

    else if(request.c == 'p')
    {
        // paste a tile
        the_game->current_map->tileArray[current_tile] = *the_game->clipboard;
    }

};

bool process_mouse_event(Game * the_game, TCOD_mouse_t request, Person *player)
{

    //set the foreground color to red for the tile the mouse is on
    Tile* moused_tile = the_game->current_map->getTileAt(the_game->mouse_evt.cx, the_game->mouse_evt.cy);
    moused_tile->tile->representation->temp_bg_color = TCODColor::red;

    return 0;
};

bool process_key_event(Game* the_game, TCOD_key_t request, Person *player)
{
    //store the last command, or use it if its needed
    // if (request == ""){
    //     request = the_game->last_cmd;
    // }
    // the_game->last_cmd = request;

    //determine if movement command
    bool incr_turn = false;
    int current_tile = player->x+(player->y*(the_game->current_map->width));

    if(is_request_move_cmd(request))
    {
        incr_turn = process_movement(the_game, request, player);
    }

    else if (is_request_basic_cmd(request))
    {
        incr_turn = process_basic_cmd(the_game, request, player);
    }

    else if(request.c == 'q')
    {
        cout << "Goodbye now" << endl;
        exit(1);
    }
    else
    {
        cout << endl << "command not found: " << char_to_str(request.c) << endl;
        cout << "Try 'help' for list of commands" << endl;
    }
    return incr_turn;
};
