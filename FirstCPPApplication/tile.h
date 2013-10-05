#ifndef TILE_H
#define TILE_H

#include <vector>

//#include "libtcod.hpp"
#include "../FirstCPPApplication/libtcod_cpp_hpp/libtcod.hpp"
#include "Representation.h"

class Map;
class Person;
class Actor;
class Object;

class Representation;
class DoorRepresentation;
class WarpRepresentation;
class FloorRepresentation;
class WallRepresentation;
class BaseRepresentation;


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

        WarpTileType() : BaseTileType()
    {
        representation = new WarpRepresentation;
    };
};


class WallTileType : public BaseTileType
{
    public: 
        WallTileType() : BaseTileType()
    {
        representation = new WallRepresentation;
    };
};


class DoorTileType : public BaseTileType
{
    public: 
        DoorTileType()  : BaseTileType() {
            tiletype;
            representation = new DoorRepresentation;
        };
};

class FloorTileType : public BaseTileType
{
    public: 
        FloorTileType()   : BaseTileType() {
            tiletype;
            representation = new FloorRepresentation;
        };
};

class Tile : public Object
{
    bool _is_occupied;
    bool _is_known;

    public:
    BaseTileType * tile;
    int tiletype;               // type of tile

    int tile_x;
    int tile_y;

    Map* map; //the map that this tile is on

    std::vector<Actor *> * occupants;       // the dude sitting on the tile
    Actor * occupant;

    bool is_occupied() { return this->_is_occupied; };

    bool is_known() { return this->_is_known; };
    void setKnown(bool is_known);

    void drawColorsToRoot(TCODColor fg_color, TCODColor bg_color);


    Tile(); 
    ~Tile(){ delete tile; };

    void updateTileType(int type = 0);
    void makeOccupied(Actor* the_actor);
    void makeUnoccupied(Actor* the_actor);
};

#endif
