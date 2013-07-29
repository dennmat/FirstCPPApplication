#include "stdafx.h"
#include "Person.h"
#include <iostream>
#include <sstream>
#include <typeinfo>
#include <cstdlib>
#include <algorithm>
#include <vector>

#include "Pet.h"

using namespace std;


void Pet::printout(){

    cout << "\nPET PRINTOUT" << endl;
    cout << "NAME: " << name << endl;
    cout << "MAX HP: " << max_hp << endl;

};

Pet::Pet(){
    name = "Nameless Pet";
    max_hp = 100;
    cur_hp = max_hp;

    representation = 'p';

    master = NULL;
    is_dead = false;
    was_attacked = false;

    attackers = new std::vector<Pet*>;
};

Pet::Pet(string name, int max_hp, char representation ){
    name = name;
    cur_hp = max_hp;
    max_hp = max_hp;

    representation = representation;

    master = NULL;
    is_dead = false;

};

void Pet::Attack(Pet* pet_target, int dmg){
    cout << "About to attack " << (*pet_target).name << endl;
    pet_target->TakeDamage(this, dmg);

    bool is_target_dead = pet_target->CheckDeath();
    if (is_target_dead){
        printf("AHAAHAHA, you're dead bro, that's got to feel bad.\n");
    };
};

void Pet::RememberAttacker(Pet* pet_attacker)
{
    if(std::find(attackers->begin(), attackers->end(), pet_attacker) != attackers->end()) 
    {
        /* v contains x */
        printf("already contains");
    }
    else 
    {
        /* v does not contain x */
        attackers->push_back(pet_attacker);
        printf("does NOT already contain");
    }

};

void Pet::TakeDamage(Pet* pet_attacker, int dmg){

    printf("About to take %d damage! ", dmg);
    printf("with %d hp.\n", cur_hp);
    cur_hp-=dmg;
    cout << name << " at " << cur_hp << " health left!" << endl << endl;

    //save attacker in history
    RememberAttacker(pet_attacker);

    if (CheckDeath()){
        printf("I've died!\n");
        //make position unblocked
        if (master != NULL)
        {
            //make the master's tile no longer occupied by him
            master->putPerson(NULL, NULL, NULL);
            //TODO: replace by corpse
        }
        else if (master == NULL)
        {
        };

    };
};

bool Pet::CheckDeath(){
    if (cur_hp <= 0  && !is_dead)
    {
        is_dead = true;
    }
    return is_dead;
};
