#pragma once

#ifndef WORLD_H
#define WORLD_H

#include <string>
#include "Person.h"
#include "game.h"

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

class BaseTileType //all tile types must inherit this
{
    public:
        bool collidable;
        char representation;    // the character that represents this tile
        string description;      // the description of the tile
        static int tiletype;
        TCODColor color;

        BaseTileType(){
			representation='b';
                    color = TCODColor::white;

        };

};


class WarpTileType : public BaseTileType
{
    public:
        int warpMap;            // if this is a warp tile, index of the map to warp to
        int warpX, warpY;       // where you warp to in the destination map

        WarpTileType()
        {
            representation = 'w';
            color = TCODColor::sepia;
        };
};


class WallTileType : public BaseTileType
{
    public: 
        WallTileType()
        {
            representation = '#';
            color = TCODColor::sepia ;
        };
};


class FloorTileType : public BaseTileType
{
    public: 
        FloorTileType(){
            representation = '.';
            tiletype;
            color = TCODColor::lighterSepia;
        };
};

class Tile
{
    bool _is_occupied;

    public:
        BaseTileType * tile;
        int tiletype;               // type of tile

        Person *occupant;       // the dude sitting on the tile

        bool is_occupied() { return _is_occupied; };


        Tile(); 
        ~Tile(){ delete tile; };

        void updateTileType(int type = 0);
        void makeOccupied();
        void makeUnoccupied();
};


class Map
{
    public:
        // C++ Classes have constructors and deconstructors that
        // are called whenever an instance of the class is created
        // or destroyed. This is often used to free any memory that was
        // allocated in the life of the instance.
        Map();
        ~Map();

        int width, height;
        int build(string filename);
        string description; // default description if tile does not have one

        int draw(Game *the_game);
        bool movePlayer(Person *thePerson, int x2, int y2);

        Tile *tileArray;
};


#endif;
