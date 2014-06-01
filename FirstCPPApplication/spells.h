#pragma once
#ifndef SPELLS_H
#define SPELLS_H

#include <string>
#include <unordered_map>
#include "game.h"
#include "actors\actor.h"
#include "enums\elements_t.h"

class AttributeContainer;
class Attribute;
class ItemEffect;
class Actor;



enum TargetTypes {
    TargettedTargetType = 0,
    GroundTargetType = 1
};

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

        int aoe;
        TargetTypes target_type;

        AttributeContainer* attrs;
        ItemEffect* spell_effect;

        Spell();
        TCODColor get_spell_color();

        void cast_spell(Actor* target);
        std::vector<Actor*> targets_for_tile(Tile* target_tile);

};


#endif
