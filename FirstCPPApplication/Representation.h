#ifndef REPRESENTATION_H
#define REPRESENTATION_H

#include "../FirstCPPApplication/libtcod_cpp_hpp/libtcod.hpp"

class Representation : public Object
{
    public:
        char repr;

        //temp fg_color for highlighting etc
        TCODColor fg_color;
        TCODColor temp_fg_color;
        TCODColor orig_fg_color;

        TCODColor bg_color;
        TCODColor temp_bg_color;
        TCODColor orig_bg_color;

        Representation(){
            repr = '/';

            fg_color = TCODColor::white;
            temp_fg_color = TCODColor::white;
            orig_fg_color = TCODColor::white;

            bg_color = TCODColor::black;
            temp_bg_color = TCODColor::white;
            orig_bg_color = TCODColor::black;
        };

};


class DeadRepresentation : public Representation
{
    public:
        DeadRepresentation(): Representation(){
            repr = 'X';
            fg_color = TCODColor::black;
        };
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
        BaseRepresentation()
        {
            repr='b';
            fg_color = TCODColor::white;
            bg_color = TCODColor::grey;
        };

};

class WarpRepresentation : public Representation
{
    public:
        WarpRepresentation() : Representation()
    {
        repr = 'w';
        fg_color = TCODColor::sepia;
    };

};

class WallRepresentation : public Representation
{
    public:
        WallRepresentation(): Representation()
    {
        repr = '#';
        fg_color = TCODColor::sepia ;
    };

};

class DoorRepresentation : public Representation
{
    public:
        DoorRepresentation(): Representation()
    {
        repr = '+';
        fg_color = TCODColor::darkerSepia;
    };

};
class FloorRepresentation : public Representation
{
    public:
        FloorRepresentation(): Representation()
    {
        repr = '.';
        fg_color = TCODColor::lighterSepia;
    };

};
#endif
