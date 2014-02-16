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
class Actor;

class TCODMap;

class Map : public Object
{
    public:
        Map();
        ~Map();

        std::vector<Actor*> enemies;  //later, this will be an array of all the enemies 

        int width, height;
        std::string description; // default description if tile does not have one

        int build_from_file(std::string filename);
        int build_from_random(int seed);

        void build_rect_room(int room_x, int room_y,
                int room_width, int room_height, int door_index);

        int draw();
        bool attackMovePlayer(Person *thePerson, int x2, int y2);

        Tile *tileArray;
        std::vector< std::vector<Tile> > *tileVector;
        std::vector<Room*> * roomVector;
        // Tile * getTileAt(int x, int y);
        Tile * getTileAt(int x, int y, bool is_original_pos=true, int ox=-1, int oy=-1);

        TCODMap* l_map;
};


#endif
