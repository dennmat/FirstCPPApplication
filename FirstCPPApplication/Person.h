#ifndef PERSON_H
#define PERSON_H
#pragma once
#include <sstream>
#include "Pet.h"
#include "Actor.h"
#include "tile.h"



class Pet;
class Tile;

class Person : public Actor
{
    public:
        std::string name;
        int age;
        int x,y;

        Pet * pet;
        bool has_live_pet;

        Tile * my_tile;

        Person();
        ~Person();

        void putPerson( Tile * next_tile, int new_x, int new_y);
        void Person::attack(Actor * target);
        void Person::update();
};
#endif
