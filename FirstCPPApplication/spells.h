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
class AttrEffect;
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

        int required_level;

        int turn_cooldown;
        int max_range;
        int min_range;
        int min_char_level;
        int cast_count;

        Actor* master;

        int aoe;
        TargetTypes target_type;

        AttributeContainer* attrs;
        AttrEffect* attr_effect;

        Spell();
        TCODColor get_spell_color();

        void cast_spell(Actor* target);
        std::vector<Actor*> targets_around_tile(Tile* target_tile);

};

class WaterBombSpell : public Spell {  public: WaterBombSpell();  };
class AutoChemHPSpell : public Spell {  public: AutoChemHPSpell();  };
class PoisonCoughSpell : public Spell {  public: PoisonCoughSpell();  };
class InnerSanctuarySpell : public Spell {  public: InnerSanctuarySpell();  };

class IceBoltSpell : public Spell {  public: IceBoltSpell(); };
class InnerHealingSpiritSpell : public Spell {  public: InnerHealingSpiritSpell(); };
class DeathsTouchSpell : public Spell {  public: DeathsTouchSpell(); };
class InnerFountainSpell : public Spell {  public: InnerFountainSpell(); };

class CorpseBlastSpell : public Spell {  public: CorpseBlastSpell(); };
class SiphonSpiritSpell : public Spell {  public: SiphonSpiritSpell(); };
class RaiseDeadSpell : public Spell {  public: RaiseDeadSpell(); };
class InnerFireSpell : public Spell {  public: InnerFireSpell(); };

class IceFistSpell : public Spell {  public: IceFistSpell(); };
class SacredKickSpell : public Spell {  public: SacredKickSpell(); };
class DeathsHandSpell : public Spell {  public: DeathsHandSpell(); };
class InnerStrengthSpell : public Spell {  public: InnerStrengthSpell(); };


#endif
