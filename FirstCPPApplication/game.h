#pragma once

#ifndef GAME_H
#define GAME_H

#include <Vector>
// #include "input.h"
// #include "libtcod_cpp_hpp\libtcod.hpp"
#include "libtcod_cpp_hpp\mouse_types.h"
#include "libtcod_cpp_hpp\console_types.h"

#include <enums\gamestate.h>
#include <enums\screens.h>


class Map;
class Tile;

class Combat;

class Actor;
class Person;
class Troll;
class BadMother;
class Jackal;
class Skeleton;
class Ogre;

class Room;
class Item;
class DebugOptions;
class Ui;

class TCODConsole;
class TCODRandom;

class Game
{
    public:
        static Person* player;     //the PC
        //static Ui* ui;
        static GameStates current_state;
        static Screens current_screen;
        static DebugOptions* debug_opts;

        static TCOD_key_t key_evt;
        static TCOD_mouse_t mouse_evt;

        static int fov_radius;
        static int enemies_size ;   //don't really know how else to get the size of the
        // static enemies list.  sizeof(type_inst_array)/sizeof(type) maybe.
        static std::vector<Actor*> enemies;  //later, this will be an array of all the enemies 
        static std::vector<Item*> items;  //later, this will be an array of all the enemies 

        static int screen_w;
        static int screen_h;

        static int map_width;
        static int map_height;
        static int town_width;
        static int town_height;

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
        static Map* build_world();
        static Map* build_town();
        static void update();

        static void update_ui();

        static void draw_ui();

        static Person*  initialize_player();
        static void  initialize_enemies();
        static void  initialize_items();
        static void fill_dungeon(Map* world);
        static void fill_town(Map* world);

        static Person * create_person(std::string name, int age, int x, int y, char repr, Map* map, std::string Combat_name = "");
        static Person * create_townsmen(std::string name, int age, int x, int y, char repr, Map* map, std::string Combat_name = "");
        static Troll * create_troll(std::string name, int age, int x, int y, char repr, Map* map, std::string Combat_name = "");
        static BadMother * create_bad_mother(std::string name, int age, int x, int y, char repr, Map* map, std::string Combat_name = "");
        static Jackal * create_jackal(std::string name, int age, int x, int y, char repr, Map* map, std::string Combat_name = "");
        static Skeleton * create_skeleton(std::string name, int age, int x, int y, char repr, Map* map, std::string Combat_name = "");
        static Ogre * create_ogre(std::string name, int age, int x, int y, char repr, Map* map, std::string Combat_name = "");

        static Tile* get_mouse_tile();
        static Tile* get_player_tile();

        template<class T>
        static T* spawn_creature( TCODRandom* spawning_rng, TCODRandom* linear_rng, Room* room);

        static void start_game();
        static void mainloop();
};


#endif
