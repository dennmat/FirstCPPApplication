#ifndef PET_H
#define PET_H
#pragma once
#include <sstream>
#include "Person.h"
#include <vector>

class Person; 

class Pet
{
    public:
    std::string name;
    int max_hp;
    int cur_hp;
    Person* master;
    char representation;

    bool is_dead;
    bool was_attacked;
    std::vector<Pet*> * attackers;

    Pet (std::string name, int max_hp, char representation = 'p');
    Pet::Pet();
    // Pet::Init();
    void Pet::Attack(Pet* pet_target, int dmg);
    void Pet::TakeDamage(Pet* pet_attacker, int dmg);
    void printout();
    bool Pet::CheckDeath();
    void Pet::RememberAttacker(Pet* pet_attacker);
};
#endif
