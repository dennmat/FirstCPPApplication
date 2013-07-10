#ifndef PET_H
#define PET_H
#pragma once
#include <sstream>

class Pet{
    public:
    std::string name;
    int max_hp;
    int cur_hp;
    Person* master;
    char representation;

    bool is_dead;

    Pet (std::string name, int max_hp, char representation = 'p');
    Pet::Pet();
    // Pet::Init();
    void Pet::Attack(Pet* target, int dmg);
    void Pet::TakeDamage(int dmg);
    void printout();
    bool Pet::CheckDeath();
};
#endif
