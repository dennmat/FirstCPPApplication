#include "stdafx.h"

#include "actors\actor.h"
#include "spells.h"
#include "attribute_container.h"
#include "attribute.h"
#include "attr_effect.h"
#include "tile.h"
#include "ui.h"
#include "messages.h"



Spell::Spell()
{
    this->name = "Unamed spell";
    this->mana_cost = 10;

    this->turn_cooldown = 1;
    this->max_range = 7;
    this->min_range = 1;
    this->min_char_level = 1;
    this->cast_count = 0;

    this->required_level = 1;

    this->attrs = new AttributeContainer;
    // this->attrs->health->current_val = 100;

    this->attr_effect = new AttrEffect;
    this->attr_effect->set_all_vals_to(0);

    this->element = elements_t::FireElement;

    this->aoe = 0;
    this->target_type = TargetTypes::TargettedTargetType;

    this->master = NULL;

};

bool Spell::is_valid_target(Tile* targetted_tile)
{
    if (Ui::is_targetting && targetted_tile->is_occupied() || this->target_type == GroundTargetType)
    {
        return true;
    }
    else
    {
        new Message(Ui::msg_handler_main, NOTYPE_MSG, "Pick an actual target how about.");
        return false;
    };
};

bool Spell::is_in_range(int distance) 
{
    if (distance < this->max_range)
    {
        return true;
    }
    else
    {
        new Message(Ui::msg_handler_main, NOTYPE_MSG, "Out of range. Max is %i, you're at %i.", this->max_range, distance);
        return false;
    };
};

bool Spell::has_enough_mana() 
{ 
    if ( this->master->attrs->mana->current_val >= this->mana_cost)
    {
        return true;
    }
    else
    {
        new Message(Ui::msg_handler_main, NOTYPE_MSG, "No mana for this cast!");
        return false;
    };
};

TCODColor Spell::get_spell_color()
{
    std::unordered_map<elements_t, TCODColor> spell_color = std::unordered_map<elements_t, TCODColor>();

    spell_color.insert(std::make_pair<elements_t, TCODColor>(FireElement, TCODColor::red));
    spell_color.insert(std::make_pair<elements_t, TCODColor>(WaterElement, TCODColor::blue));
    spell_color.insert(std::make_pair<elements_t, TCODColor>(DeathElement, TCODColor::darkestGrey));
    spell_color.insert(std::make_pair<elements_t, TCODColor>(LifeElement, TCODColor::lightGreen));
    spell_color.insert(std::make_pair<elements_t, TCODColor>(SpectreElement, TCODColor::darkPurple));
    spell_color.insert(std::make_pair<elements_t, TCODColor>(CrystalElement, TCODColor::magenta));

    return spell_color.at(this->element);
};

void Spell::cast_spell(Actor* target)
{
    this->attr_effect->ApplyAllEffects(target);

    if (this->attr_effect->duration > 0)
    {
        TimedEffect* timed_effect = new TimedEffect;
        timed_effect->effect = this->attr_effect;
        timed_effect->turn_applied = Game::turn_count;
        target->timed_spell_effects->push_back(timed_effect);
    };

};


std::vector<Actor*> Spell::targets_around_tile(Tile* target_tile)
{
    typedef std::vector<Tile*> tile_vector;
    std::vector<Actor*> targets;
    if (this->aoe > 0)
    {
        if (this->aoe != 1) printf("aoe is only directly adjacent tiles, anything higher than 1 is ignored\n");
        std::vector<Tile*>* adjacent_tiles = target_tile->getAdjacentTiles(1);
        for (tile_vector::iterator it = adjacent_tiles->begin(); it != adjacent_tiles->end(); it++)
        {
            if ((*it)->is_occupied())
            {
                targets.push_back((*it)->occupant);
            }
        };
        delete adjacent_tiles;
    };
    if (target_tile->occupant != NULL) // assuming NULL if they died
    {
        targets.push_back(target_tile->occupant);
    }

    return targets;

};

/* fighter */

WaterBombSpell::WaterBombSpell() : Spell()
{
    this->name = "Water Bomb";
    this->element = WaterElement;
    this->attr_effect->health_current_val = -7;
    this->mana_cost = 5;
    this->max_range = 11;
};

AutoChemHPSpell::AutoChemHPSpell() : Spell()
{
    this->name = "AutoChemistry: Health Potion";
    this->element = LifeElement;
    this->attr_effect->health_current_val = 15;
    this->mana_cost = 5;
    this->max_range = 1;

    this->required_level = 4;
};

PoisonCoughSpell::PoisonCoughSpell() : Spell()
{
    this->name = "Poison Cough";
    this->element = DeathElement;
    this->attr_effect->health_current_val = -25;
    this->mana_cost = 20;
    this->max_range = 2;
    this->required_level = 6;
};

InnerSanctuarySpell::InnerSanctuarySpell() : Spell()
{
    this->required_level = 8;
    this->name = "Inner Sanctuary";
    this->element = LifeElement;
    this->attr_effect->health_max_val = 30;
    this->mana_cost = 20;
    this->max_range = 1;
    this->attr_effect->duration = 50;
};

/* mage */

IceBoltSpell::IceBoltSpell() : Spell()
{

    this->required_level = 2;
    this->name = "Ice bolt";
    this->element = WaterElement;
    this->attr_effect->health_current_val = -9;
    this->mana_cost = 7;
    this->max_range = 11;
};

InnerHealingSpiritSpell::InnerHealingSpiritSpell() : Spell()
{

    this->required_level = 4;
    this->name = "Inner Healing Spirit";
    this->element = LifeElement;
    this->attr_effect->health_current_val = 15;
    this->mana_cost = 5;
    this->max_range = 1;
};

DeathsTouchSpell::DeathsTouchSpell() : Spell()
{

    this->required_level = 6;
    this->name = "Death's Touch";
    this->element = DeathElement;
    this->attr_effect->health_current_val = -25;
    this->mana_cost = 20;
    this->max_range = 2;
};

InnerFountainSpell::InnerFountainSpell() : Spell()
{

    this->required_level = 8;
    this->name = "Inner Fountain";
    this->element = CrystalElement;
    this->attr_effect->mana_regen_rate = 5;
    this->mana_cost = 20;
    this->max_range = 1;
    this->attr_effect->duration = 50;
};

/* necromancy */

CorpseBlastSpell::CorpseBlastSpell() : Spell()
{

    this->required_level = 2;
    this->name = "Corpse Blast";
    this->element = DeathElement;
    this->attr_effect->health_current_val = -9;
    this->mana_cost = 7;
    this->max_range = 11;
};

SiphonSpiritSpell::SiphonSpiritSpell() : Spell()
{

    this->required_level = 4;
    this->name = "Siphon Spirit";
    this->element = DeathElement;
    this->attr_effect->health_current_val = 15;
    this->mana_cost = 5;
    this->max_range = 1;
};

RaiseDeadSpell::RaiseDeadSpell() : Spell()
{

    this->required_level = 6;
    this->name = "Raise the dead";
    this->element = DeathElement;
    this->attr_effect->health_current_val = -25;
    this->mana_cost = 20;
    this->max_range = 2;
};

InnerFireSpell::InnerFireSpell() : Spell()
{

    this->required_level = 8;
    this->name = "Inner Fire";
    this->element = FireElement;
    this->attr_effect->mana_regen_rate = 5;
    this->mana_cost = 20;
    this->max_range = 1;
    this->attr_effect->duration = 50;
};


/* brawler */

IceFistSpell::IceFistSpell() : Spell()
{

    this->required_level = 2;
    this->name = "Ice fist";
    this->element = WaterElement;
    this->attr_effect->health_current_val = -7;
    this->mana_cost = 5;
    this->max_range = 2;
};

SacredKickSpell::SacredKickSpell() : Spell()
{

    this->required_level = 4;
    this->name = "Sacred Kick";
    this->element = LifeElement;
    this->attr_effect->health_current_val = -15;
    this->mana_cost = 5;
    this->max_range = 3;
};

DeathsHandSpell::DeathsHandSpell() : Spell()
{

    this->required_level = 6;
    this->name = "Death's Hand";
    this->element = DeathElement;
    this->attr_effect->health_current_val = -25;
    this->mana_cost = 20;
    this->max_range = 2;
};

InnerStrengthSpell::InnerStrengthSpell() : Spell()
{

    this->required_level = 8;
    this->name = "Inner Strength";
    this->element = DeathElement;
    this->attr_effect->damage_current_val = 15;
    this->mana_cost = 20;
    this->max_range = 1;
    this->attr_effect->duration = 23;
};

