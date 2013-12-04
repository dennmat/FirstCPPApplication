#pragma once
#ifndef COMBAT_H
#define COMBAT_H
#include <sstream>
#include <vector>
// #include "actor.h"
//#include "Person.h"

class Person; 
class Game;

class Combat 
{
    private:
        std::string default_name;
    public:
        // std::string name;
        Person* master;
        char representation;

        bool is_dead;
        bool was_attacked;
        std::vector<Combat*> * attackers;

        Combat (std::string name, int max_hp, Person* master, char representation = 'p' );
        //Combat::Combat();
        // Combat::Init();
        void Combat::Attack(Combat* Combat_target, int dmg);
        void Combat::TakeDamage(Combat* Combat_attacker, int dmg);
        void printout();
        bool Combat::CheckDeath();
        void Combat::RememberAttacker(Combat* Combat_attacker, bool mark_the_attk);
        Combat* Combat::GetLastAttacker();
        void Combat::update(Game* game);
        void Combat::assign_to_master(Person* master);
};
#endif
