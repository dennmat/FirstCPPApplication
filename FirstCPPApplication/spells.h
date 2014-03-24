#pragma once
#ifndef SPELLS_H
#define SPELLS_H

#include <string>
#include <unordered_map>

class AttributeContainer;
class Attribute;
class ItemEffect;

#include "enums\elements_t.h"

class Spell 
{
    public:

        static const std::unordered_map<elements_t, TCODColor>* spell_color;

        std::string name;
        int mana_cost;

        elements_t element;

        int turn_cooldown;
        int max_range;
        int min_range;
        int min_char_level;
        int cast_count;

        AttributeContainer* attrs;
        ItemEffect* spell_effect;

        Spell();
        TCODColor get_spell_color();
};


#endif
