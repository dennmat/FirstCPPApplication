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

    screen_w = 40;
    screen_h = 25;

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

    TCODConsole::initRoot(screen_w, screen_h, "FirstCPPApplication", false);

    bool battle_done = false;

    current_map->draw(this);
    while ( !TCODConsole::isWindowClosed() ) {
        TCODConsole::flush();
        TCOD_key_t key_evt;
        TCOD_mouse_t mouse_evt;
        TCOD_event_t evt = TCODSystem::waitForEvent(TCOD_EVENT_KEY_PRESS, &key_evt, &mouse_evt, false);
        process_event(key_evt, &player);
        current_map->draw(this);

    }

    std::cout << "Hit enter to exit" << std::endl;
    // std::cin.get();
};
