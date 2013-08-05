#pragma once

#ifndef WORLD_H
#define WORLD_H

#include "object.h"
#include <string>
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


class Tile;

class Map : public Object
{
    public:
        Map();
        ~Map();

        int width, height;
        int build(string filename);
        string description; // default description if tile does not have one

        int draw(Game *the_game);
        bool movePlayer(Person *thePerson, int x2, int y2);

        Tile *tileArray;
        Tile * Map::getTileAt(int x, int y);

        TCODMap* l_map;
};

inline const char * const BoolToString(bool b)
{
    return b ? "true" : "false";
}

#endif;
