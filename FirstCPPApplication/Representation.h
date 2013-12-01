#ifndef REPRESENTATION_H
#define REPRESENTATION_H

// #include "../FirstCPPApplication/libtcod_cpp_hpp/libtcod.hpp"


#include "Object.h"

class Object;

class TCODColor;

class Representation : public Object
{
    public:
        char repr;

        //temp fg_color for highlighting etc
         TCODColor* fg_color;
         TCODColor* temp_fg_color;
         TCODColor* orig_fg_color;
                    
         TCODColor* bg_color;
         TCODColor* temp_bg_color;
         TCODColor* orig_bg_color;

        Representation();

};


class DeadRepresentation : public Representation
{
    public:
        DeadRepresentation();
};

// class FloorRepresentation : public Representation
// {
//     FloorRepresentation(){
//         repr = 'X';
//         fg_color = TCODColor::black;
//     };
// };

class BaseRepresentation : public Representation
{
    public:
        BaseRepresentation();

};

class BaseItemRepresentation : public Representation
{
    public:
        BaseItemRepresentation() ;

};

class WarpRepresentation : public Representation
{
    public:
        WarpRepresentation();

};

class WallRepresentation : public Representation
{
    public:
        WallRepresentation();

};

class DoorRepresentation : public Representation
{
    public:
        DoorRepresentation();

};
class FloorRepresentation : public Representation
{
    public:
        FloorRepresentation();

};
#endif
