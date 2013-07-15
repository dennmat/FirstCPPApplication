#ifndef TILE_H
#define TILE_H

#include "libtcod.hpp"
#include "person.h"

class BaseTileType //all tile types must inherit this
{
    public:
        bool collidable;
        char representation;    // the character that represents this tile
        std::string description;      // the description of the tile
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

class Tile : public Object
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

#endif
