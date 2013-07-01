#include "stdafx.h"
#include <iostream>
#include <sstream>
#include <typeinfo>
#include <cstdlib>
#include <algorithm>

#include "utils.h"
#include "Person.h"
#include "Pet.h"
#include "FirstCPPApplication.h"
#include "world.h"

using namespace std;

Map newmap;

void WelcomeMessage(){

    printf("\t\tWELCOME TO THE GAME!\n");
    printf("\t\t********************\n");
    // printf("\n\n");
    printf("\n\t\t   Prepare to die!\n\n");

};

void process_movement(string request, Person *player)
{

	// Movement || N, S, E, W ||

    if(request == "north" || request == "n")
	{
		if(newmap.movePlayer(player, 0, -1))
			player->y--;
	}

	else if(request == "south" || request == "s")
	{
		if(newmap.movePlayer(player, 0, 1))
			player->y++;
	}

	else if(request == "east" || request == "e")
	{
		if(newmap.movePlayer(player, 1, 0))
			player->x++;
	}

	else if(request == "west" || request == "w")
	{
		if(newmap.movePlayer(player, -1, 0))
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

void process_request(string request, Person *player)
{

    //determine if movement command
    bool is_move_cmd;
    is_move_cmd = is_request_move_cmd(request);

	if(request == "test")
	{
		cout << "I see you testin'" << endl;
	}


    if(is_move_cmd){
        process_movement(request, player);
    }

	else if(request == "warp" || request == "r")
	{
		// currentmap = MAP_ARRAY[tileArray[thePerson->x+(thePerson->y*height)].warpMap]
		// player->x = currentmap->startx;
		// player->y = currentmap->starty;
	}

	else if(request == "help" || request == "h")
	{
		cout << "-------------------" << endl;
		cout << "Available Commands:" << endl;
		cout << "[H]elp	-	See Help" <<endl;
		cout << "[N]orth	-	Move North" <<endl;
		cout << "[S]outh	-	Move South" <<endl;
		cout << "[E]ast	-	Move East" <<endl;
		cout << "[W]est	-	Move West" <<endl;
		cout << "[Q]uit	-	Quit" <<endl;
		cout << "-------------------" << endl;
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

    std::string arr[] = {"first", "Second", "third", "fourth"};

    // std::string* res;
    // res = find(arr, arr+3, "first");

    cout << sizeof(arr)/sizeof(string) << endl;


    WelcomeMessage();

    //init player
    Person player1;
    player1.name = "Josh";
    player1.age = 23;
	player1.x = 3;
	player1.y = 3;

	newmap.build(&player1);

    Pet p1_pet;
    p1_pet.master = &player1;

    //init enemy
    Pet enemy_pet;
    Person enemy_player;
    enemy_pet.master = &enemy_player;
    enemy_player.name = "Max";
    enemy_player.age = 50;

    // save space for the command output
    cout << endl;
    cout << endl;
    cout << endl;

    bool battle_done = false;

    while (!battle_done){

		newmap.draw(&player1);
        std::string answer = ask_for_str("What would you like to do?\n");
		system("cls");
		WelcomeMessage();
        answer = ToLower(answer);

		process_request(answer, &player1);

       /* if (answer == "n" || answer == "no"){
            battle_done = true;
            printf("Fine be that way\n");
            break;
        }

        int damage  = ask_for_int("how much damage do you deal?\n");
        p1_pet.Attack(&enemy_pet, damage);
	  */
    }

    //attack player


    std::cout << "Hit enter to exit" << std::endl;
    // std::cin.ignore();
    std::cin.get();

    return 0;
}
