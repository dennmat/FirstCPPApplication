#pragma once
#ifndef ITEM_EFFECT_H
#define ITEM_EFFECT_H

#include <vector>
#include <iostream>

class Actor;
class Person;
class Combat;
class Person;
class Item;
class Inventory;
class Tile;
class Item;
class Attribute;

class ItemEffect
{
    public:

        int health_current_val;
        int health_max_val;
        int health_regen_rate;
        int health_regen_interval; 

        int mana_current_val;
        int mana_max_val;
        int mana_regen_rate;
        int mana_regen_interval; 

        int armor_current_val;
        int armor_max_val;
        int armor_regen_rate;
        int armor_regen_interval; 

        int damage_current_val;
        int damage_max_val;
        int damage_regen_rate;
        int damage_regen_interval; 

        std::vector<Actor*>* applied_to;

        ItemEffect();
        void ApplyHealthEffects(Actor* actor);
        void ApplyManaEffects(Actor* actor);
        void ApplyArmorEffects(Actor* actor);
        void ApplyDamageEffects(Actor* actor);
        void RemoveHealthEffects(Actor* actor);
        void RemoveManaEffects(Actor* actor);
        void RemoveArmorEffects(Actor* actor);
        void RemoveDamageEffects(Actor* actor);

        bool already_applied(Actor* actor);
        void mark_applied_to(Actor* actor);

        std::string c_str();
};

#endif
