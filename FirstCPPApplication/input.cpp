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



using namespace std;

// Game the_game;



bool process_movement(Game* the_game, TCOD_key_t request, Person *player)
{
    Map *world = the_game->world;
    bool buildmode = the_game->buildmode;
    // Movement || N, S, E, W ||


    if(request.c == 'n' && request.pressed)
    {
        player->is_moving_up = true;
        if(the_game->current_map->movePlayer(player, 0, -1) || buildmode)
        { 
            //            player->y--;
            return true;
        }
    }
    else { player->is_moving_up = false; };

    if(request.c == 's' && request.pressed)
    {
        player->is_moving_down = true;
        if(the_game->current_map->movePlayer(player, 0, 1) || buildmode)
        { 
            //            player->y++; 
            return true;
        }
    }
    else { player->is_moving_down = false; };

    if(request.c == 'e' && request.pressed)
    {
        player->is_moving_right = true;
        if(the_game->current_map->movePlayer(player, 1, 0) || buildmode)
        {
            //            player->x++; 
            return true;
        }

    }
    else { player->is_moving_right = false; };

    if(request.c == 'w' && request.pressed)
    {
        player->is_moving_left = true;
        if(the_game->current_map->movePlayer(player, -1, 0) || buildmode)
        { 
            //            player->x--; 
            return true;
        }
    }
    else 
    { player->is_moving_left = false; };

    return false;

};

bool is_request_move_cmd(TCOD_key_t request){

    bool is_move_cmd;
    is_move_cmd = false;

    // using ~ (tilde) as a dead character as a stand in for noop
    char move_cmds[] = { 'n', 's', 'e', 'w', '~' };

    int move_cmds_size = sizeof(move_cmds)/sizeof(char);

    char* result;
    result = find(move_cmds, move_cmds+(move_cmds_size-1), request.c);

    if (*result != '~') { is_move_cmd = true; }



    return is_move_cmd;

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
    bool is_move_cmd;
    bool incr_turn = false;
    int current_tile = player->x+(player->y*(the_game->current_map->width));
    is_move_cmd = is_request_move_cmd(request);

    // process_buildmode(request, current_tile);

    if(is_move_cmd){
        incr_turn =process_movement(the_game, request, player);
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

        cout << endl << "command not found: " << temp_str << endl;
        cout << "Try 'help' for list of commands" << endl;
    }
    return incr_turn;
};
