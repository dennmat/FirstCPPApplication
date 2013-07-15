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

//creates a person and places them on the current map
Person * Game::create_person(string name, int age, int x, int y, char repr)
{

    Person * new_pers = new Person;
    new_pers->name = name;
    new_pers->age = age;
    new_pers->x = x;
    new_pers->y = y;
    new_pers->representation = repr;

    Tile * next_tile = &current_map->tileArray[x + (y*current_map->width)];
    new_pers->putPerson(NULL, next_tile, x, y);

    // cout << "created person " << name << endl;

    return new_pers;

};

//creates a bunch of enemies on the map
void  Game::initialize_enemies(){
    
    enemies = create_person("first", 99, 5, 5, 'a');
    ++enemies = create_person("second", 66, 5, 6, 'a');


};

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
    initialize_enemies(); // create the enemies

    last_cmd = "not set";

};

void Game::mainloop()
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
