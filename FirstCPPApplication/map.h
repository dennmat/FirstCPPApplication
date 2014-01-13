#pragma once

#ifndef WORLD_H
#define WORLD_H

#define     TILE_NORMAL     0
#define     TILE_WALL       1

#include <string>
#include <vector>

#include "object.h"


class Game;
class Room;
class Person;
class Object;
class Tile;

class TCODMap;

using namespace std;
class Map : public Object
{
    public:
        Map();
        ~Map();

        int width, height;
        int build_from_file(string filename);
        int build_from_random(int seed);

        Game* the_game;

        // void add_room(
        void build_rect_room(int room_x, int room_y,
                int room_width, int room_height, int door_index);
        string description; // default description if tile does not have one

        int draw(Game *the_game);
        bool attackMovePlayer(Person *thePerson, int x2, int y2);

        Tile *tileArray;
        vector< vector<Tile> > *tileVector;
        vector<Room*> * roomVector;
        // Tile * getTileAt(int x, int y);
        Tile * getTileAt(int x, int y, bool is_original_pos=true, int ox=-1, int oy=-1);

        TCODMap* l_map;
};


#endif
