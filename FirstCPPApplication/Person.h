#ifndef PERSON_H
#define PERSON_H
#pragma once
#include <sstream>
#include <string>

#include "Actor.h"
#include "Pet.h"


class Pet;
class Tile;
class Actor;
class Game;

class Person : public Actor
{
    public:
        // std::string name;
        int age;

        Pet * pet;
        bool has_live_pet;



        Person();
        Person(std::string name, int age, int x, int y, char repr, std::string pet_name);
        ~Person();

        void attack(Actor * target);
        void update(Game* game);
};
#endif
