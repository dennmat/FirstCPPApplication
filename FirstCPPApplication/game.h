#pragma once

#ifndef GAME_H
#define GAME_H

#include <Vector>
// #include "input.h"
#include "libtcod_cpp_hpp\libtcod.hpp"

#include <enums\gamestate.h>


class Map;
class Tile;
class Person;
class Combat;
class Actor;
class Troll;
class Skeleton;
class Item;
class DebugOptions;

class Ui;


class Game
{
    public:
        static Person* player;     //the PC
        //static Ui* ui;
        static GameStates current_state;
        static DebugOptions* debug_opts;

        static TCOD_key_t key_evt;
        static TCOD_mouse_t mouse_evt;

        static int enemies_size ;   //don't really know how else to get the size of the
        // static enemies list.  sizeof(type_inst_array)/sizeof(type) maybe.
        static std::vector<Actor*> enemies;  //later, this will be an array of all the enemies 
        static std::vector<Item*> items;  //later, this will be an array of all the enemies 

        static int screen_w;
        static int screen_h;

        static int map_width;
        static int map_height;
        static int view_width;
        static int view_height;

        static int camera_w;
        static int camera_h;
        static int camera_x;
        static int camera_y;

        static unsigned long int turn_count;
        static unsigned long long int tick_count;

        static int fps_limit; //how many frames do you want to refresh at a second

        static TCODConsole* game_console;
        static Map *world;
        static Map *current_map;
        static int current_map_index;

        static bool buildmode;
        static Tile *clipboard;


        static std::string last_cmd;

        // Game();
        static void buildworld();
        static void update();

        static void update_ui();

        static void draw_ui();

        static Person*  initialize_player();
        static void  initialize_enemies();
        static void  Game::initialize_items();

        static Person * create_person(std::string name, int age, int x, int y, char repr, std::string Combat_name = "");
        static Troll * create_troll(std::string name, int age, int x, int y, char repr, std::string Combat_name = "");
        static Skeleton * create_skeleton(std::string name, int age, int x, int y, char repr, std::string Combat_name = "");


        static void mainloop();
};

#endif
