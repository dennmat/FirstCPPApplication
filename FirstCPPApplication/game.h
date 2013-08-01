#ifndef GAME_H
#define GAME_H
#pragma once

#include "stdafx.h"
#include "Person.h"
#include "Pet.h"
#include "world.h"
#include "tile.h"

#include <Vector>

class Map;
class Tile;

class Game
{
    public:
        Person* player;     //the PC

        int enemies_size ;   //don't really know how else to get the size of the
        // enemies list.  sizeof(type_inst_array)/sizeof(type) maybe.
        std::vector<Actor*> enemies;  //later, this will be an array of all the enemies 

        int screen_w;
        int screen_h;

        long int turn_count;


        Map *world;
        Map *current_map;
        int current_map_index;

        bool buildmode;
        Tile *clipboard;

        std::string last_cmd;

        Game::Game();
        void Game::buildworld();
        void Game::update();

        Person*  Game::initialize_player();
        void  Game::initialize_enemies();

        Person * Game::create_person(std::string name, int age, int x, int y, char repr, std::string pet_name = "");


        void Game::mainloop();
};

#endif
