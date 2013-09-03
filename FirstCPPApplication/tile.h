#ifndef TILE_H
#define TILE_H

#include <vector>

//#include "libtcod.hpp"
#include "../FirstCPPApplication/libtcod_cpp_hpp/libtcod.hpp"

#include "person.h"
#include "Representation.h"
#include "map.h"


class Map;


class BaseTileType //all tile types must inherit this
{
    public:
        bool collidable;
        Representation * representation;    // the character that represents this tile
        std::string description;      // the description of the tile
        static int tiletype;
        TCODColor color;


        BaseTileType()
        {
            representation = new BaseRepresentation;
        };

};


class WarpTileType : public BaseTileType
{
    public:
        int warpMap;            // if this is a warp tile, index of the map to warp to
        int warpX, warpY;       // where you warp to in the destination map

        WarpTileType()
        {
            representation = new WarpRepresentation;
        };
};


class WallTileType : public BaseTileType
{
    public: 
        WallTileType()
        {
            representation = new WallRepresentation;
        };
};


class DoorTileType : public BaseTileType
{
    public: 
        DoorTileType(){
            tiletype;
            representation = new DoorRepresentation;
        };
};

class FloorTileType : public BaseTileType
{
    public: 
        FloorTileType(){
            tiletype;
            representation = new FloorRepresentation;
        };
};

class Tile : public Object
{
    bool _is_occupied;

    public:
    BaseTileType * tile;
    int tiletype;               // type of tile

    int tile_x;
    int tile_y;

    Map* map; //the map that this tile is on

    std::vector<Actor *> * occupants;       // the dude sitting on the tile
    Actor * occupant;

    bool is_occupied() { return this->_is_occupied; };


    Tile(); 
    ~Tile(){ delete tile; };

    void updateTileType(int type = 0);
    void makeOccupied(Actor* the_actor);
    void makeUnoccupied(Actor* the_actor);
};

#endif
