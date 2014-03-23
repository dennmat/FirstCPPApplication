#pragma once
#ifndef SPELLS_H
#define SPELLS_H

#include <string>

class AttributeContainer;
class Attribute;
class ItemEffect;

class Spell 
{
    public:
        std::string name;
        int mana_cost;

        int turn_cooldown;
        int max_range;
        int min_range;
        int min_char_level;
        int cast_count;

        AttributeContainer* attrs;
        ItemEffect* spell_effect;

        Spell();
};

#endif
