#include "stdafx.h"
#include "game.h"
#include "Person.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include "world.h"
#include "FirstCPPApplication.h"
#include "utils.h"

#include "libtcod.hpp"


void Game:: buildworld()
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
            world[i].build(line);
        }
    }
}


Person  Game::initialize_player(){

    player.name = "Josh";
    player.age = 23;
    player.x = 3;
    player.y = 3;
    player.representation = '@';

    //Pet p1_pet;
    //p1_pet.master = &player;

    Tile * next_tile = &current_map->tileArray[player.x + (player.y*current_map->width)];
    player.putPerson(NULL, next_tile, player.x, player.y);

    return player;

};

Game::Game()
{

    enemies_size = 255; //hardcoded
    buildmode = false;

    buildworld();
    current_map = world; //I'm not so sure about this, but it solved the mem issue

    initialize_player(); //created the Person player

    last_cmd = "not set";

};

void Game::start_game()
{
    WelcomeMessage();

    // save space for the command output
    // cout << endl;
    // cout << endl;
    // cout << endl;
    TCODConsole::initRoot(80,50,"libtcod C++ tutorial",false);

    bool battle_done = false;

    while ( !TCODConsole::isWindowClosed() ) {
        TCOD_key_t key;
        // TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS,&key,NULL);
        //TCODConsole::waitForKeypress(true);
        key = TCODConsole::checkForKeypress();
        current_map->draw(this);
        // std::string answer = ask_for_str("What would you like to do?\n"); //disable input on regular console window for draggable TCODConsole
        clearScreen(); //gotta get rid of that flash when the page redraws
        WelcomeMessage();
        // answer = ToLower(answer);

        process_request(key, &player);
    }

    std::cout << "Hit enter to exit" << std::endl;
    // std::cin.get();
};
