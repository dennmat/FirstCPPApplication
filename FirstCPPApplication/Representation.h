#ifndef REPRESENTATION_H
#define REPRESENTATION_H

#include "../FirstCPPApplication/libtcod_cpp_hpp/libtcod.hpp"

class Representation : public Object
{
    public:
        char repr;
        TCODColor color;

        Representation(){
            repr = '/';
            color = TCODColor::white;
        };

};


class DeadRepresentation : public Representation
{
    public:
    DeadRepresentation(){
        repr = 'X';
        color = TCODColor::black;
    };
};

// class FloorRepresentation : public Representation
// {
//     FloorRepresentation(){
//         repr = 'X';
//         color = TCODColor::black;
//     };
// };

class BaseRepresentation : public Representation
{
    public:
    BaseRepresentation()
    {
        repr='b';
        color = TCODColor::white;
    };

};

class WarpRepresentation : public Representation
{
    public:
    WarpRepresentation()
    {
        repr = 'w';
        color = TCODColor::sepia;
    };

};

class WallRepresentation : public Representation
{
    public:
    WallRepresentation()
    {
        repr = '#';
        color = TCODColor::sepia ;
    };

};

class FloorRepresentation : public Representation
{
    public:
    FloorRepresentation()
    {
        repr = '.';
        color = TCODColor::lighterSepia;
    };

};
#endif
