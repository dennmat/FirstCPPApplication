#pragma once

#include "stdafx.h"
#include "Person.h"
#include "Pet.h"
#include "world.h"

class Map;
class Tile;

class Game
{
    public:
        Person player;     //the PC

        int enemies_size ;   //don't really know how else to get the size of the
                            // enemies list.  sizeof(type_inst_array)/sizeof(type) maybe.
        Person *enemies;  //later, this will be an array of all the enemies 

        Map *world;
        Map *current_map;
        int current_map_index;

        bool buildmode;
        Tile *clipboard;

        std::string last_cmd;

        Game::Game();
        void Game::buildworld();

        Person  Game::intialize_player();
};

