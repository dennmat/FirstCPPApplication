#include "stdafx.h"
#include "game.h"
#include "Person.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include "world.h"


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


Person  Game::intialize_player(){

    player.name = "Josh";
    player.age = 23;
    player.x = 3;
    player.y = 3;
    player.representation = '@';

    Pet p1_pet;
    p1_pet.master = &player;

    return player;


};

Game::Game(){

    enemies_size = 255; //hardcoded
    buildmode = false;

    intialize_player(); //created the Person player
    buildworld();
    current_map = world; //I'm not so sure about this, but it solved the mem issue

    last_cmd = "not set";

};
