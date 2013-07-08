#pragma once

#ifndef WORLD_H
#define WORLD_H
// The preprocessor directives shown above ensure that
// if this file is included in more than one file the
// linker only tried to read it once, and won't show
// duplicate function definition errors
#include <string>
#include "Person.h"
#include "game.h"

/***************************** world.h ******************************
 *  Written by                                                      *
 *      Grant Mcneil                                                *
 *                                                                  *
 * please note:                                                     *
 * this code is covered by the GrantPL license.                     *
 * any project or construct that contains this file is immediatly   *
 * at the ownership of Grant McNeil.                                    *
 ********************************************************************/

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

class BaseTileType //all tile types must inherit this
{
    public:
        bool collidable;
        char representation;    // the character that represents this tile
        string description;      // the description of the tile

};

class WarpTileType : public BaseTileType
{
    public:
        int warpMap;            // if this is a warp tile, index of the map to warp to
        int warpX, warpY;       // where you warp to in the destination map
};

class WallTileType : public BaseTileType
{
};

class FloorTileType : public BaseTileType
{
};

class Tile
{
    public:
        BaseTileType * tile;
        int tiletype;               // type of tile
        Person *occupant;       // the dude sitting on the tile
        Tile(){ tile = new BaseTileType;};
};


// in order to properly draw the player, i'm going to store
// whats under the player in tempchar
class Map
{
    public:
        // C++ Classes have constructors and deconstructors that
        // are called whenever an instance of the class is created
        // or destroyed. This is often used to free any memory that was
        // allocated in the life of the instance.
        Map();
        ~Map();
        int build(string filename);
        string description; // default description if tile does not have one
        int draw(Game *the_game);
        int width, height;
        bool movePlayer(Person *thePerson, int x2, int y2);
        Tile *tileArray;
        char tempchar;
};


#endif;
