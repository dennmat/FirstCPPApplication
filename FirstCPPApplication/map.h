#pragma once

#ifndef WORLD_H
#define WORLD_H

#include "object.h"
#include <string>
#include <vector>
#include "Person.h"
#include "game.h"
#include "tile.h"

#include "libtcod.hpp"


/********************************************************************
  So the idea here is to have a world, which is a navigatable set of
  "maps", which are grids, 2-dimensional array of "tiles." A (one
  dimensional) array of maps would be created, and special "warp" tiles
  on these maps would be used to send the player from one map to another.
 ********************************************************************/
using namespace std;

#define     TILE_NORMAL     0
#define     TILE_WALL       1

class Game;
class Room;


class Tile;

class Map : public Object
{
    public:
        Map();
        ~Map();

        int width, height;
        int Map::build_from_file(string filename);
        int Map::build_from_random(int seed);

        // void Map::add_room(
        void Map::build_rect_room(int room_x, int room_y,
                int room_width, int room_height, int door_count);
        string description; // default description if tile does not have one

        int Map::draw(Game *the_game);
        bool Map::movePlayer(Person *thePerson, int x2, int y2);

        Tile *tileArray;
        vector< vector<Tile> > *tileVector;
        vector<Room*> * roomVector;
        // Tile * Map::getTileAt(int x, int y);
        Tile * Map::getTileAt(int x, int y, bool is_original_pos=true, int ox=-1, int oy=-1);

        TCODMap* l_map;
};

inline const char * const BoolToString(bool b, bool verbose=true)
{
    if (verbose)
    return b ? "true" : "false";
    else
    return b ? "T" : "F";
}

#endif;
