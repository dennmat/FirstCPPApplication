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

struct type_applied_s 
{
    bool current_val;
    bool max_val;
    bool regen_rate;
    bool interval;
    bool all;
};

struct applied_to_s 
{
    type_applied_s health;
    type_applied_s mana;
    type_applied_s armor;
    type_applied_s damage;
    Actor* actor;
};

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

        applied_to_s  applied_to;
        std::vector<applied_to_s*>* actors_applied_to;

        ItemEffect();
        void ApplyAllEffects(Actor* actor);
        void ApplyHealthEffects(Actor* actor);
        void ApplyManaEffects(Actor* actor);
        void ApplyArmorEffects(Actor* actor);
        void ApplyDamageEffects(Actor* actor);

        void RemoveAllEffects(Actor* actor);
        void RemoveHealthEffects(Actor* actor);
        void RemoveManaEffects(Actor* actor);
        void RemoveArmorEffects(Actor* actor);
        void RemoveDamageEffects(Actor* actor);

        bool already_applied_all(Actor* actor);
        bool already_applied_health(Actor* actor);
        bool already_applied_mana(Actor* actor);
        bool already_applied_armor(Actor* actor);
        bool already_applied_damage(Actor* actor);

        void mark_applied_all(Actor* actor);
        void mark_applied_health(Actor* actor);
        void mark_applied_mana(Actor* actor);
        void mark_applied_armor(Actor* actor);
        void mark_applied_damage(Actor* actor);

        std::string ItemEffect::small_convert(std::string prefix, long double val);
        std::string full_str();
        std::string ItemEffect::oneline_str();

        void set_all_vals_to(int new_val);
        void set_health_vals_to(int new_val);
        void set_mana_vals_to(int new_val);
        void set_armor_vals_to(int new_val);
        void set_damage_vals_to(int new_val);
};

#endif
