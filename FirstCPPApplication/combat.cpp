#include "stdafx.h"
#include <iostream>
#include <sstream>
#include <typeinfo>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <string>

#include <libtcod.hpp>

#include "combat.h"
#include "attribute_container.h"
#include <actors\Person.h>
#include <actors\actor.h>
#include "attribute.h"
#include "tile.h"
#include <Representation.h>
#include "ui.h"
#include "messages.h"



void Combat::printout(){

    std::cout << "\nCombat PRINTOUT" << std::endl;
    //cout << "NAME: " << name << endl;
    std::cout << "MAX HP: " << this->master->attrs->health->max_val << std::endl;

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

Combat::Combat(std::string name, int max_hp, Person* master, char representation)
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

void Combat::update()
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

void Combat::LevelUp(int levels)
{
    //reset, accounting for left over exp
    this->master->xp_this_level = this->master->xp_required - this->master->xp_this_level;

    this->master->level++;
    std::cout << "NEW LEVEL IS: " << this->master->level << std::endl;

    this->master->attrs->health->current_val+=10;
    this->master->attrs->health->max_val+=10;

    this->master->attrs->damage->current_val+=3;
    this->master->attrs->damage->max_val+=3;

    Tile* tile = this->master->my_tile;

    std::vector<Tile*>* adj_tiles = tile->getAdjacentTiles(2);
    for (std::vector<Tile*>::iterator it = adj_tiles->begin(); it != adj_tiles->end(); ++it)
    {
        (*it)->tile->representation->temp_bg_color = (TCODColor*)(&TCODColor::gold);
    };
    std::cout << "just exploded with color for a frame" << std::endl;
};

void Combat::GiveExp(int exp_to_gain)
{
    this->master->xp += exp_to_gain;
    this->master->xp_this_level += exp_to_gain;
    // calc if level up
    if (this->master->xp_this_level > this->master->xp_required)
    {
        this->LevelUp();
    };
};

void Combat::Attack(Combat* combat_target, int dmg){
    //cout << "About to attack " << (*combat_target).name << endl;
    new Message(Ui::msg_handler_main, "About to attack %s for %d damage.", (*combat_target).master->name.c_str(), dmg);
    combat_target->TakeDamage(this, dmg);

    bool is_target_dead = combat_target->CheckDeath();
    if (is_target_dead){
        //get opponents exp value
        int exp_to_gain = combat_target->master->xp_value;
        //add it to the master's exp
        this->GiveExp(exp_to_gain);
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
        master->Die();
    }
    else if (master == NULL)
    {
        printf("I've no master so he's not going to die, is he?\n");
    };

};

Combat* Combat::GetLastAttacker()
{
    std::cout << "*** Retaliation ***" << std::endl;
    Combat * assailant;
    assailant = attackers->back();
    // cout << "attacker: " << assailant->name << endl;

    return assailant;
};

void Combat::TakeDamage(Combat* combat_attacker, int dmg)
{

    int adjusted_dmg = dmg-this->master->attrs->armor->current_val;
    (this->master->attrs->health->current_val)-= std::max(adjusted_dmg, 1);

    std::cout << this->master->name;
    std::cout << " took " << dmg << " damage! ";
    std::cout << "with " << this->master->attrs->health->current_val << "hp left.";
    std::cout << std::endl;

    //save attacker in history
    this->RememberAttacker(combat_attacker);

    if (this->CheckDeath())
    {
        this->Die();
    };
};

bool Combat::CheckDeath()
{
    if (this->master->attrs->health->current_val <= 0  && !is_dead)
    {
        is_dead = true;
    }
    return is_dead;
};
