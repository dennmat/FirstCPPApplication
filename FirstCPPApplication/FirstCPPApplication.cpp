#include "stdafx.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <typeinfo>
#include <cstdlib>
#include <algorithm>

#include <windows.h>

#include "utils.h"
#include "Person.h"
#include "Pet.h"
#include "FirstCPPApplication.h"
#include "world.h"
#include "game.h"


#include "libtcod.hpp"

using namespace std;

Game the_game;

void WelcomeMessage(){
    printf("\t\tWELCOME TO THE GAME!\n");
    printf("\t\t********************\n");
    printf("\n\t\t   Prepare to die!\n\n");
};


void process_movement(string request, Person *player)
{
    Map *world = the_game.world;
    bool buildmode = the_game.buildmode;
    // Movement || N, S, E, W ||

    if(request == "north" || request == "n")
    {
        if(the_game.current_map->movePlayer(player, 0, -1) || buildmode)
        { 
//            player->y--;
        }
    }

    else if(request == "south" || request == "s")
    {
        if(the_game.current_map->movePlayer(player, 0, 1) || buildmode)
        { 
//            player->y++; 
        }
    }

    else if(request == "east" || request == "e")
    {
        if(the_game.current_map->movePlayer(player, 1, 0) || buildmode)
        {
//            player->x++; 
        }

    }

    else if(request == "west" || request == "w")
    {
        if(the_game.current_map->movePlayer(player, -1, 0) || buildmode)
        { 
//            player->x--; 
        }
    }

};

bool is_request_move_cmd(string request){

    bool is_move_cmd;
    is_move_cmd = false;

    string move_cmds[] = { "north", "n",
        "south", "s",
        "east", "e",
        "west", "w",
        "noop"
    };

    int move_cmds_size = sizeof(move_cmds)/sizeof(string);

    string* result;
    result = find(move_cmds, move_cmds+(move_cmds_size-1), request);

    if (*result != "noop")
    {
        is_move_cmd = true;
    }

    return is_move_cmd;

};

void process_buildmode(string request, int current_tile)
{
    Map *world = the_game.world;
    bool buildmode = the_game.buildmode;
    if(request == "change" || request == "c")
    {
        // do the stuff to make a new tile
        cout << "tiletype: ";
        Tile this_tile = the_game.current_map->tileArray[current_tile];
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
        cin >> this_tile.tile->representation;
    }
    else if(request == "write" || request == "i")
    {
        // Write the map to a file
        ofstream myfile;
        string filename;
        cout << "Filename: ";
        getline(cin, filename); // do this twice because hitting enter... whatever
        myfile.open (filename);
        int i,j;

        // Map this_map = the_game.current_map;
        myfile << the_game.current_map->width << endl;
        myfile << the_game.current_map->height << endl;  
        myfile << the_game.current_map->description << endl;  

        for(i=0; i<the_game.current_map->height; i++)
            for(j=0; j<the_game.current_map->width; j++)
            {
                Tile active_tile = the_game.current_map->tileArray[(i*the_game.current_map->width)+j];
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

    else if(request == "copy" || request == "o")
    {
        // Copy a tile
        the_game.clipboard = &the_game.current_map->tileArray[current_tile];
    }

    else if(request == "paste" || request == "p")
    {
        // paste a tile
        the_game.current_map->tileArray[current_tile] = *the_game.clipboard;
    }

};

void process_request(string request, Person *player)
{
    //store the last command, or use it if its needed
    if (request == ""){
        request = the_game.last_cmd;
    }
    the_game.last_cmd = request;

    //determine if movement command
    bool is_move_cmd;
    int current_tile = player->x+(player->y*the_game.current_map->width);
    is_move_cmd = is_request_move_cmd(request);

    if(request == "test")
    {
        cout << "I see you testin'" << endl;
    }

    process_buildmode(request, current_tile);

    if(is_move_cmd){
        process_movement(request, player);
    }

    else if(request == "buildmode" || request == "b")
    {
        the_game.buildmode=!the_game.buildmode;
    }

    else if(request == "warp" || request == "r")
    {
        Tile *this_tile = &the_game.current_map->tileArray[current_tile];
        if(this_tile->tiletype == 2)
        {
            WarpTileType* warp_tile;
            warp_tile = (WarpTileType*) this_tile->tile;

            the_game.current_map_index = warp_tile->warpMap;
            the_game.current_map = &(the_game.world[warp_tile->warpMap]);

            player->x = warp_tile->warpX;
            player->y = warp_tile->warpY;
            // player->x = currentmap->startx;
            // player->y = currentmap->starty;
        }
    }

    else if(request == "help" || request == "h")
    {
        system("cls");
        if(the_game.buildmode)
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
        cin.get();
        system("cls");
    }

    else if(request == "quit" || request == "q")
    {
        exit(1);
    }
    else
    {
        cout << endl << "command not found: " << request << endl;
        cout << "Try 'help' for list of commands" << endl;
    }
};

int* getConsoleSize(){

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int columns, rows;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    // printf("columns: %d\n", columns);
    // printf("rows: %d\n", rows);

    int *info = new int[2];
    info[0] = columns;
    info[1] = rows;
    return info;
};


void clearScreen()
{
    // the current way
    system("cls");

    // //my attempted way
    //     int* info = getConsoleSize();
    //     int columns = info[0];
    //     
    //     int i = 0;
    //     cout << string(columns, '\n');
    //     // while (i < columns){
    //     //     cout << endl;
    //     //     i++;
    //     // }
    // 
}

// //took this from the web to remove the screen flash, but it doesn't help.
// {   
//     HANDLE                     hStdOut;
//     CONSOLE_SCREEN_BUFFER_INFO csbi;
//     DWORD                      count;
//     DWORD                      cellCount;
//     COORD                      homeCoords = { 0, 0 };
// 
//     hStdOut = GetStdHandle( STD_OUTPUT_HANDLE );
//     if (hStdOut == INVALID_HANDLE_VALUE) return;
// 
//     /* Get the number of cells in the current buffer */
//     if (!GetConsoleScreenBufferInfo( hStdOut, &csbi )) return;
//     cellCount = csbi.dwSize.X *csbi.dwSize.Y;
// 
//     /* Fill the entire buffer with spaces */
//     if (!FillConsoleOutputCharacter(
//                 hStdOut,
//                 (TCHAR) ' ',
//                 cellCount,
//                 homeCoords,
//                 &count
//                 )) return;
// 
//     /* Fill the entire buffer with the current colors and attributes */
//     if (!FillConsoleOutputAttribute(
//                 hStdOut,
//                 csbi.wAttributes,
//                 cellCount,
//                 homeCoords,
//                 &count
//                 )) return;
// 
//     /* Move the cursor home */
//     SetConsoleCursorPosition( hStdOut, homeCoords );
// }


void libtcod()
{
   int playerx=40,playery=25;
    TCODConsole::initRoot(80,50,"libtcod C++ tutorial",false);
    while ( !TCODConsole::isWindowClosed() ) {
        TCOD_key_t key;
        TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS,&key,NULL);
        switch(key.vk) {
            case TCODK_UP : playery--; break;
            case TCODK_DOWN : playery++; break;
            case TCODK_LEFT : playerx--; break;
            case TCODK_RIGHT : playerx++; break;
            default:break;
        }
        TCODConsole::root->clear();
        TCODConsole::root->putChar(playerx,playery,'@');
        TCODConsole::flush();
    }
};

int main ()
{

    libtcod();

    the_game.start_game();

    return 0;
}
