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

using namespace std;

enum directions_t {
    NW=0, N, NE,
    W,    X,  E,
    SW,   S, SE,
    NO_MATCH
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
            return directions_t::NO_MATCH;
        }
        return it->second;
    }
    else
    {
        auto it = spec_movemap.find(key.vk);
        if(it == spec_movemap.end())
        {
            return directions_t::NO_MATCH;
        }
        return it->second;
    }
    // return directions_t::N;
};

//returns whether or not the player has moved
bool process_movement(Game* the_game, TCOD_key_t request, Person *player)
{
    Map *world = the_game->world;
    bool buildmode = the_game->buildmode;
    // Movement || N, S, E, W ||

    int plr_x, plr_y;
    plr_x = player->x;
    plr_y = player->y;

    if (request.pressed == false)
    {
        player->is_moving_up = false; 
        player->is_moving_right = false;
        player->is_moving_down = false;
        player->is_moving_left = false;
        return false;

    };

    if( direction_pressed(request) == directions_t::N && request.pressed)
        // if(direction_pressed(request) == directions_t::N && request.pressed)
    {
        player->is_moving_up = true;
        if(the_game->current_map->attackMovePlayer(player, 0, -1) || buildmode)
        { 
            //player->y--;
            return true;
        }
    }

    if(request.c == 's' && request.pressed)
    {
        player->is_moving_down = true;
        if(the_game->current_map->attackMovePlayer(player, 0, 1) || buildmode)
        { 
            //player->y++; 
            return true;
        }
    }

    if(request.c == 'e' && request.pressed)
    {
        player->is_moving_right = true;
        if(the_game->current_map->attackMovePlayer(player, 1, 0) || buildmode)
        {
            //player->x++; 
            return true;
        }

    }

    if(request.c == 'w' && request.pressed)
    {
        player->is_moving_left = true;
        if(the_game->current_map->attackMovePlayer(player, -1, 0) || buildmode)
        { 
            //player->x--; 
            return true;
        }
    }


    //if the player has moved or attacked this update, increment the turn
    //counter
    if ((plr_x != player->x || plr_y != player->y || player->has_attacked) && request.pressed) {
        return true;
    }
    else {
        return false;
    }

};

bool is_request_move_cmd(TCOD_key_t request){
    return direction_pressed(request) != directions_t::NO_MATCH;
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

    // process_buildmode(request, current_tile);

    if(is_request_move_cmd(request)){
        incr_turn = process_movement(the_game, request, player);
    }

    else if(request.c == 'h')
    {
        if(the_game->buildmode)
        {
            cout << "-------------------" << endl;
            cout << "Available Commands:" << endl;
            cout << "[H]elp -   See Help" <<endl;
            cout << "[N]orth    -   Move North" <<endl;
            cout << "[S]outh    -   Move South" <<endl;
            cout << "[E]ast -   Move East" <<endl;
            cout << "[W]est -   Move West" <<endl;
            cout << "[C]hange - Alter a Tile" <<endl;
            cout << "Wr[i]te -  Write map to file" <<endl;
            cout << "C[O]py -   Copy a Tile" <<endl;
            cout << "[P]aste -  Paste a Tile" <<endl;
            cout << "[Q]uit -   Quit" <<endl;
            cout << "-------------------" << endl;
        }
        else
        {
            cout << "-------------------" << endl;
            cout << "Available Commands:" << endl;
            cout << "[H]elp -   See Help" <<endl;
            cout << "[N]orth    -   Move North" <<endl;
            cout << "[S]outh    -   Move South" <<endl;
            cout << "[E]ast -   Move East" <<endl;
            cout << "[W]est -   Move West" <<endl;
            cout << "[Q]uit -   Quit" <<endl;
            cout << "-------------------" << endl;
        }
    }

    else if(request.c == 'q')
    {
        cout << "Goodbye now" << endl;
        exit(1);
    }
    else
    {
        //convert key char to string. 
        stringstream ss;
        ss << request.c;
        string temp_str;
        ss >> temp_str;

        cout << endl << "command not found: " << char_to_str(request.c) << endl;
        cout << "Try 'help' for list of commands" << endl;
    }
    return incr_turn;
};
