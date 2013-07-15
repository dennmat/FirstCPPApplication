#ifndef PERSON_H
#define PERSON_H
#pragma once
#include <sstream>
#include "Pet.h"
#include "Actor.h"



class Pet;
class Tile;

class Person : public Actor
{
    public:
        std::string name;
        int age;

        int x,y;
        char representation;

        bool is_fighter;
        Pet * pet;

        Person();
        ~Person();

        void putPerson(Tile * current_tile, Tile * next_tile, int new_x, int new_y);
        void Person::attack(Person * target);
};
#endif
