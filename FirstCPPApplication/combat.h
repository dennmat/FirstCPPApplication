#pragma once
#ifndef COMBAT_H
#define COMBAT_H
#include <sstream>
#include <vector>
// #include <actors/actor.h>
// #include <actors\Person.h>

class Person; 
class Actor; 
class Game;

class Combat 
{
    private:
        std::string default_name;
    public:
        // std::string name;
        Actor* master;
        char representation;

        bool is_dead;
        bool was_attacked;
        std::vector<Combat*> * attackers;

        Combat (std::string name, int max_hp, Person* master, char representation = 'p' );

        void Attack(Combat* Combat_target, int dmg);
        void TakeDamage(Combat* Combat_attacker, int dmg);
        bool CheckDeath();
        void RememberAttacker(Combat* Combat_attacker, bool mark_the_attk);
        void Die();
        Combat* GetLastAttacker();
        void update();
        void assign_to_master(Person* master);
        void GiveExp(int exp_to_gain);
        void LevelUp(int levels = 1);

        void printout();
};
#endif
