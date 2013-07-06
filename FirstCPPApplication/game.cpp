#include "stdafx.h"
#include "game.h"
#include "Person.h"
#include <iostream>
#include <sstream>
#include <fstream>


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

    Person player1;
    player1.name = "Josh";
    player1.age = 23;
    player1.x = 3;
    player1.y = 3;
    player1.representation = '@';


    Pet p1_pet;
    p1_pet.master = &player1;

    return player1;


};

Game::Game(){

    enemies_size = 255; //hardcoded
    buildmode = false;

    player = &(intialize_player());
    // player = &player1;
    buildworld();
    current_map = world; //I'm not so sure about this, but it solved the mem issue

};
