#include "stdafx.h"
#include <iostream>
#include <sstream>
#include <typeinfo>
#include <cstdlib>
#include <algorithm>
#include <vector>

#include "combat.h"
#include "Person.h"
#include "attribute_container.h"
#include "attribute.h"
#include "Actor.h"

using namespace std;


void Combat::printout(){

    cout << "\nCombat PRINTOUT" << endl;
    //cout << "NAME: " << name << endl;
    cout << "MAX HP: " << this->master->attrs->health->max_val << endl;

};

//Combat::Combat(){
//    default_name = "Nameless Combat";
//    //name = default_name;
//
//    representation = 'p';
//
//    max_hp = NULL;
//    cur_hp = NULL;
//    master = NULL;
//    is_dead = false;
//    was_attacked = false;
//
//    attackers = new std::vector<Combat*>;
//};

void Combat::assign_to_master(Person* master)
{
    this->master = master;
    // this->max_hp = (this->master->attrs->health->max_val);
    // this->cur_hp = (this->master->attrs->health->current_val);

    //if (name == default_name)
    //{
    //    // cout << "named default name: " << name << endl;
    //    // cout << "renaming to masters name" << endl;
    //    name = master->name + "'s Combat";
    //    // cout << "new name: " << name << endl;
    //}
    //else
    //{
    //    cout << "not named default, it's named instead: " << name << endl;
    //};
};

Combat::Combat(string name, int max_hp, Person* master, char representation)
{
   //name = name;

   // this->cur_hp = max_hp;
   // this->max_hp = max_hp;

   representation = representation;

   master = NULL;
   is_dead = false;

   attackers = new std::vector<Combat*>;
   was_attacked = false;

};

void Combat::update(Game* game)
{
    if (was_attacked == true)
    {
        //cout << "I (a Combat), " << name << ", was attacked, ouch!" << endl;

        // //retaliate
        // Combat* assailant = GetLastAttacker();
        // Attack(assailant, 1);


        // //calm the Combat down
        // was_attacked = false;
    };
};

void Combat::Attack(Combat* combat_target, int dmg){
    //cout << "About to attack " << (*combat_target).name << endl;
    combat_target->TakeDamage(this, dmg);

    bool is_target_dead = combat_target->CheckDeath();
    if (is_target_dead){
        printf("Hahaa!, you're dead bro, that's got to feel bad.\n");
    };
};

void Combat::RememberAttacker(Combat* combat_attacker, bool mark_the_attk=true)
{
    if (mark_the_attk == true) { was_attacked = true; };

    if(std::find(attackers->begin(), attackers->end(), combat_attacker) != attackers->end()) 
    {
        // printf("I've already been attacked by you.\n");
    }
    else 
    {
        attackers->push_back(combat_attacker);
        // printf("Oh hi, you're new.\n");
    }

};

void Combat::Die()
{
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
            printf("I've no master\n");
        };

};

Combat* Combat::GetLastAttacker()
{
    cout << "*** Retaliation ***" << endl;
    Combat * assailant;
    assailant = attackers->back();
    // cout << "attacker: " << assailant->name << endl;

    return assailant;
};

void Combat::TakeDamage(Combat* combat_attacker, int dmg){

    printf("About to take %d damage! ", dmg);
    printf("with %d hp.\n", this->master->attrs->health->current_val);
    (this->master->attrs->health->current_val)-=dmg;
    //cout << name << " at " << cur_hp << " health left!" << endl << endl;

    //save attacker in history
    this->RememberAttacker(combat_attacker);

    if (this->CheckDeath()){
        this->Die();

    };
};

bool Combat::CheckDeath(){
    if (this->master->attrs->health->current_val <= 0  && !is_dead)
    {
        is_dead = true;
    }
    return is_dead;
};
