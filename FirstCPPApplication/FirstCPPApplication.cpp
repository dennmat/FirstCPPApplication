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

Map *world;
Map newmap;
int current_map;
bool buildmode=false;
Tile clipboard;

void WelcomeMessage(){

    printf("\t\tWELCOME TO THE GAME!\n");
    printf("\t\t********************\n");
    // printf("\n\n");
    printf("\n\t\t   Prepare to die!\n\n");

};

void buildworld(Person person1, Person person2)
{
    string line;
    ifstream myfile ("world.txt");
    int num_of_worlds;
    int i;

    if (myfile.is_open())
    {
	// get width
	getline (myfile,line);
	num_of_worlds = atoi(line.c_str());

	world = new Map[num_of_worlds];

	for(i=0;i<num_of_worlds;i++)
	{
	    // get height
	    getline (myfile,line);
	    world[i].build(&person1, &person2, line);
	}
	
	
    }
}

void process_movement(string request, Person *player)
{

    // Movement || N, S, E, W ||

    if(request == "north" || request == "n")
    {
	if(world[current_map].movePlayer(player, 0, -1) || buildmode)
	    player->y--;
    }

    else if(request == "south" || request == "s")
    {
	if(world[current_map].movePlayer(player, 0, 1) || buildmode)
	    player->y++;
    }

    else if(request == "east" || request == "e")
    {
	if(world[current_map].movePlayer(player, 1, 0) || buildmode)
	    player->x++;
    }

    else if(request == "west" || request == "w")
    {
	if(world[current_map].movePlayer(player, -1, 0) || buildmode)
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
	if(request == "change" || request == "c")
    {
		// do the stuff to make a new tile
		cout << "tiletype: ";
		cin >> world[current_map].tileArray[current_tile].tiletype;
		if(world[current_map].tileArray[current_tile].tiletype == 2)
		{
			cout << "Warp Map: ";
			cin >> world[current_map].tileArray[current_tile].warpMap;
			cout << "WarpX: ";
			cin >> world[current_map].tileArray[current_tile].warpX;
			cout << "WarpY: ";
			cin >> world[current_map].tileArray[current_tile].warpY;
		}
		cout << endl << "Description: ";
		getline(cin, world[current_map].tileArray[current_tile].description);	// do this twice because hitting enter... whatever
		getline(cin, world[current_map].tileArray[current_tile].description);
		cout << endl << "Representation: ";
		cin >> world[current_map].tileArray[current_tile].representation;
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

		myfile << world[current_map].width << endl;
		myfile << world[current_map].height << endl;  
		myfile << world[current_map].description << endl;  

		for(i=0; i<world[current_map].height; i++)
			for(j=0; j<world[current_map].width; j++)
			{
				myfile << world[current_map].tileArray[(i*world[current_map].width)+j].representation << endl;
				myfile << world[current_map].tileArray[(i*world[current_map].width)+j].tiletype << endl;
				if(world[current_map].tileArray[(i*world[current_map].width)+j].tiletype == 2)
				{
					myfile << world[current_map].tileArray[(i*world[current_map].width)+j].warpMap << endl;
					myfile << world[current_map].tileArray[(i*world[current_map].width)+j].warpX << endl;
					myfile << world[current_map].tileArray[(i*world[current_map].width)+j].warpY << endl;
				}
				myfile << world[current_map].tileArray[(i*world[current_map].width)+j].description << endl;  
			}
		myfile.close();
	}

	else if(request == "copy" || request == "o")
    {
		// Copy a tile
		clipboard = world[current_map].tileArray[current_tile];
	}

	else if(request == "paste" || request == "p")
    {
		// paste a tile
		world[current_map].tileArray[current_tile] = clipboard;
	}

}

void process_request(string request, Person *player)
{

    //determine if movement command
    bool is_move_cmd;
	int current_tile = player->x+(player->y*world[current_map].width);
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
		buildmode=!buildmode;
	}

    else if(request == "warp" || request == "r")
    {
	
	if(world[current_map].tileArray[current_tile].tiletype == 2)
	{
	    current_map = world[current_map].tileArray[current_tile].warpMap;
	
	    player->x = world[current_map].tileArray[current_tile].warpX;
	    player->y = world[current_map].tileArray[current_tile].warpY;
	    // player->x = currentmap->startx;
	    // player->y = currentmap->starty;
	}
    }

    else if(request == "help" || request == "h")
    {
		system("cls");
		if(buildmode)
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
}

int main ()
{

    WelcomeMessage();

    //init player
    Person player1;
    player1.name = "Josh";
    player1.age = 23;
    player1.x = 3;
    player1.y = 3;
    player1.representation = '@';


    Pet p1_pet;
    p1_pet.master = &player1;

    //init enemy
    Person enemy_player;
    enemy_player.x = 1;
    enemy_player.y = 3;
    enemy_player.representation = 'E';

    Pet enemy_pet;
    enemy_pet.master = &enemy_player;
    enemy_player.name = "Max";
    enemy_player.age = 50;

    Game the_game;
    the_game.player = &player1;


    buildworld(player1, enemy_player);
    

    // save space for the command output
    cout << endl;
    cout << endl;
    cout << endl;


    bool battle_done = false;

    while (!battle_done){

	world[current_map].draw(&the_game);
	std::string answer = ask_for_str("What would you like to do?\n");
	system("cls");
	WelcomeMessage();
	answer = ToLower(answer);

	process_request(answer, &player1);

    }

    std::cout << "Hit enter to exit" << std::endl;
    std::cin.get();

    return 0;
}
