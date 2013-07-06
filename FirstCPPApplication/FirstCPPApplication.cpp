#include "stdafx.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <typeinfo>
#include <cstdlib>
#include <algorithm>

#include "utils.h"
#include "Person.h"
#include "Pet.h"
#include "FirstCPPApplication.h"
#include "world.h"
#include "game.h"

using namespace std;

Game the_game;

void WelcomeMessage(){

    printf("\t\tWELCOME TO THE GAME!\n");
    printf("\t\t********************\n");
    // printf("\n\n");
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
            player->y--;
    }

    else if(request == "south" || request == "s")
    {
        if(the_game.current_map->movePlayer(player, 0, 1) || buildmode)
            player->y++;
    }

    else if(request == "east" || request == "e")
    {
        if(the_game.current_map->movePlayer(player, 1, 0) || buildmode)
            player->x++;
    }

    else if(request == "west" || request == "w")
    {
        if(the_game.current_map->movePlayer(player, -1, 0) || buildmode)
            player->x--;
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
			cout << "Warp Map: ";
			cin >> this_tile.warpMap;
			cout << "WarpX: ";
			cin >> this_tile.warpX;
			cout << "WarpY: ";
			cin >> this_tile.warpY;
		}
		cout << endl << "Description: ";
		getline(cin, this_tile.description);	// do this twice because hitting enter... whatever
		getline(cin, this_tile.description);
		cout << endl << "Representation: ";
		cin >> this_tile.representation;
	}
	else if(request == "write" || request == "i")
    {
		// Write the map to a file
		ofstream myfile;
		string filename;
		cout << "Filename: ";
		getline(cin, filename);	// do this twice because hitting enter... whatever
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
				myfile << active_tile.representation << endl;
				myfile << active_tile.tiletype << endl;
				if(active_tile.tiletype == 2)
				{
					myfile << active_tile.warpMap << endl;
					myfile << active_tile.warpX << endl;
					myfile << active_tile.warpY << endl;
				}
				myfile << active_tile.description << endl;  
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

}

void process_request(string request, Person *player)
{
    Map *world = the_game.world;

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
	
	if(the_game.current_map->tileArray[current_tile].tiletype == 2)
	{
	    the_game.current_map_index = the_game.current_map->tileArray[current_tile].warpMap;
        the_game.current_map = &(the_game.world[the_game.current_map_index]);
	
	    player->x = the_game.current_map->tileArray[current_tile].warpX;
	    player->y = the_game.current_map->tileArray[current_tile].warpY;
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
			cout << "C[O]py -	Copy a Tile" <<endl;
			cout << "[P]aste -	Paste a Tile" <<endl;
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
	cout << endl << "command not found" << endl;
	cout << "Try 'help' for list of commands" << endl;
    }
};


// Person intialize_player(){
// 
//     Person player1;
//     player1.name = "Josh";
//     player1.age = 23;
//     player1.x = 3;
//     player1.y = 3;
//     player1.representation = '@';
// 
// 
//     Pet p1_pet;
//     p1_pet.master = &player1;
// 
//     return player1;
// 
// 
// };
// 
int main ()
{
    // the_game.player = &the_game.intialize_player(); //reinitializes the already created player. 
	//Otherwise it gets corrupted or otherwise invalid values between this main() call
    // and the global declaration of it above, right after line 473 in the builtin crtexe.c file.


    WelcomeMessage();

    //init player
    // Person player1 = intialize_player();
    // the_game.player = &player1;



    // save space for the command output
    cout << endl;
    cout << endl;
    cout << endl;


    bool battle_done = false;

    while (!battle_done){

	the_game.current_map->draw(&the_game);
	std::string answer = ask_for_str("What would you like to do?\n");
	system("cls");
	WelcomeMessage();
	answer = ToLower(answer);

	process_request(answer, &the_game.player);

    }

    std::cout << "Hit enter to exit" << std::endl;
    std::cin.get();

    return 0;
}
