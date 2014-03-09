#pragma once
#ifndef PERSON_H
#define PERSON_H
#include <sstream>
#include <string>

#include "Actor.h"


class Combat;
class Tile;
class Actor;
class Game;

class Person : public Actor
{
    public:
        // std::string name;
        int age;

        Combat * combat;
        bool has_live_combat;



        Person();
        Person(std::string name, int age, int x, int y, char repr, std::string combat_name);
        ~Person();

        void attack(Actor * target);
        void update();
        void championize();
        void talk_to(Actor* occupant);
};
#endif
