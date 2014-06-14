#include "stdafx.h"
#include "class.h"
#include "actors\actor.h"
#include "attribute.h"
#include "attribute_container.h"
#include "ui.h"
#include "messages.h"
#include "spells.h"
#include "attr_effect.h"
#include "item.h"

void IClass::LevelUpHealth(double change)
{
    this->master->attrs->health->current_val+=change;
    this->master->attrs->health->max_val+=change;
};
void IClass::LevelUpMana(double change)
{
    this->master->attrs->mana->current_val+=change;
    this->master->attrs->mana->max_val+=change;
};
void IClass::LevelUpArmor(double change)
{
    this->master->attrs->armor->current_val+=change;
    this->master->attrs->armor->max_val+=change;
};
void IClass::LevelUpDamage(double change)
{
    this->master->attrs->damage->current_val+=change;
    this->master->attrs->damage->max_val+=change;
};

FighterClass::FighterClass()
{
    this->type = 1;
    this->name = "Fighter";
    this->description = "Basic Class, all around decent guy.";
};

void FighterClass::LevelUpStats(int levels)
{
    int i = 0;
    while (i < levels)
    {
        this->LevelUpHealth(2);
        this->LevelUpMana(2);
        this->LevelUpArmor(0.25);
        this->LevelUpDamage(0.25);
        i++;
    };
};

void FighterClass::LevelUpSkills(int levels)
{
    Spell* new_spell;
    switch (this->master->level)
    {
        case 2:
            // new Message(Ui::msg_handler_main, MOOD_MSG, "%c%c%c%cYou've learned a new spell!%c", TCOD_COLCTRL_FORE_RGB, (int)TCODColor::blue.r, (int)TCODColor::blue.g, (int)TCODColor::blue.b, TCOD_COLCTRL_STOP);
            new Message(Ui::msg_handler_main, MOOD_MSG, "You've learned a new technique!");
            new_spell = new Spell;
            new_spell->master = this->master;
            this->master->spells->push_back(new_spell);
            new_spell->name = "Water Bomb";
            new_spell->element = WaterElement;
            new_spell->attr_effect->health_current_val = -7;
            new_spell->mana_cost = 5;
            new_spell->max_range = 11;
            break;

        case 4:
            new Message(Ui::msg_handler_main, MOOD_MSG, "You've learned a new technique!");
            new_spell = new Spell;
            new_spell->master = this->master;
            this->master->spells->push_back(new_spell);
            new_spell->name = "AutoChemistry: Health Potion";
            new_spell->element = LifeElement;
            new_spell->attr_effect->health_current_val = 15;
            new_spell->mana_cost = 5;
            new_spell->max_range = 1;
            break;

        case 6:
            new Message(Ui::msg_handler_main, MOOD_MSG, "You've learned a new technique!");
            new_spell = new Spell;
            new_spell->master = this->master;
            this->master->spells->push_back(new_spell);
            new_spell->name = "Poison Cough";
            new_spell->element = DeathElement;
            new_spell->attr_effect->health_current_val = -25;
            new_spell->mana_cost = 20;
            new_spell->max_range = 2;
            break;

        case 8:
            new Message(Ui::msg_handler_main, MOOD_MSG, "You've learned a new technique!");
            new_spell = new Spell;
            new_spell->master = this->master;
            this->master->spells->push_back(new_spell);
            new_spell->name = "Inner Sanctuary";
            new_spell->element = LifeElement;
            new_spell->attr_effect->health_max_val = 30;
            new_spell->mana_cost = 20;
            new_spell->max_range = 1;
            new_spell->attr_effect->duration = 50;
            break;

    };

};

MageClass::MageClass()
{
    this->type = 2;
    this->name = "Mage";
    this->description = "Magic Class, feels more spiritual.";
};

void MageClass::LevelUpStats(int levels)
{
    this->LevelUpHealth(2);
    this->LevelUpMana(4);
    this->LevelUpArmor(0.1);
    this->LevelUpDamage(0.1);
};

void MageClass::LevelUpSkills(int levels)
{
    Spell* new_spell;
    switch (this->master->level)
    {
        case 2:
            // new Message(Ui::msg_handler_main, MOOD_MSG, "%c%c%c%cYou've learned a new spell!%c", TCOD_COLCTRL_FORE_RGB, (int)TCODColor::blue.r, (int)TCODColor::blue.g, (int)TCODColor::blue.b, TCOD_COLCTRL_STOP);
            new_spell = new Spell;
            new_spell->master = this->master;
            new Message(Ui::msg_handler_main, MOOD_MSG, "You've learned a new spell!");
            this->master->spells->push_back(new_spell);
            new_spell->name = "Ice bolt";
            new_spell->element = WaterElement;
            new_spell->attr_effect->health_current_val = -9;
            new_spell->mana_cost = 7;
            new_spell->max_range = 11;
            break;

        case 4:
            new Message(Ui::msg_handler_main, MOOD_MSG, "You've learned a new spell!");
            new_spell = new Spell;
            new_spell->master = this->master;
            this->master->spells->push_back(new_spell);
            new_spell->name = "Inner Healing Spirit";
            new_spell->element = LifeElement;
            new_spell->attr_effect->health_current_val = 15;
            new_spell->mana_cost = 5;
            new_spell->max_range = 1;
            break;

        case 6:
            new Message(Ui::msg_handler_main, MOOD_MSG, "You've learned a new spell!");
            new_spell = new Spell;
            new_spell->master = this->master;
            this->master->spells->push_back(new_spell);
            new_spell->name = "Death's Touch";
            new_spell->element = DeathElement;
            new_spell->attr_effect->health_current_val = -25;
            new_spell->mana_cost = 20;
            new_spell->max_range = 2;
            break;

        case 8:
            new Message(Ui::msg_handler_main, MOOD_MSG, "You've learned a new spell!");
            new_spell = new Spell;
            new_spell->master = this->master;
            this->master->spells->push_back(new_spell);
            new_spell->name = "Inner Fountain";
            new_spell->element = CrystalElement;
            new_spell->attr_effect->mana_regen_rate = 5;
            new_spell->mana_cost = 20;
            new_spell->max_range = 1;
            new_spell->attr_effect->duration = 50;
            break;

    };

};

NecromancerClass::NecromancerClass()
{
    this->type = 4;
    this->name = "Necromancer";
    this->description = "Undead Class, has a thing for the dead and buried.";
};

void NecromancerClass::LevelUpStats(int levels)
{
    this->LevelUpHealth(2);
    this->LevelUpMana(4);
    this->LevelUpArmor(0.1);
    this->LevelUpDamage(0.1);
};

void NecromancerClass::LevelUpSkills(int levels)
{
    Spell* new_spell;
    switch (this->master->level)
    {
        case 2:
            // new Message(Ui::msg_handler_main, MOOD_MSG, "%c%c%c%cYou've learned a new spell!%c", TCOD_COLCTRL_FORE_RGB, (int)TCODColor::blue.r, (int)TCODColor::blue.g, (int)TCODColor::blue.b, TCOD_COLCTRL_STOP);
            new_spell = new Spell;
            new_spell->master = this->master;
            new Message(Ui::msg_handler_main, MOOD_MSG, "You've learned a new spell!");
            this->master->spells->push_back(new_spell);
            new_spell->name = "Ice bolt";
            new_spell->element = WaterElement;
            new_spell->attr_effect->health_current_val = -9;
            new_spell->mana_cost = 7;
            new_spell->max_range = 11;
            break;

        case 4:
            new Message(Ui::msg_handler_main, MOOD_MSG, "You've learned a new spell!");
            new_spell = new Spell;
            new_spell->master = this->master;
            this->master->spells->push_back(new_spell);
            new_spell->name = "Inner Healing Spirit";
            new_spell->element = LifeElement;
            new_spell->attr_effect->health_current_val = 15;
            new_spell->mana_cost = 5;
            new_spell->max_range = 1;
            break;

        case 6:
            new Message(Ui::msg_handler_main, MOOD_MSG, "You've learned a new spell!");
            new_spell = new Spell;
            new_spell->master = this->master;
            this->master->spells->push_back(new_spell);
            new_spell->name = "Death's Touch";
            new_spell->element = DeathElement;
            new_spell->attr_effect->health_current_val = -25;
            new_spell->mana_cost = 20;
            new_spell->max_range = 2;
            break;

        case 8:
            new Message(Ui::msg_handler_main, MOOD_MSG, "You've learned a new spell!");
            new_spell = new Spell;
            new_spell->master = this->master;
            this->master->spells->push_back(new_spell);
            new_spell->name = "Inner Fountain";
            new_spell->element = CrystalElement;
            new_spell->attr_effect->mana_regen_rate = 5;
            new_spell->mana_cost = 20;
            new_spell->max_range = 1;
            new_spell->attr_effect->duration = 50;
            break;

    };

};

BrawlerClass::BrawlerClass()
{
    this->type = 3;
    this->name = "Brawler";
    this->description = "Beefy Class, a bit of a meathead.";
};

void BrawlerClass::LevelUpStats(int levels)
{
    this->LevelUpHealth(4);
    this->LevelUpMana(1);
    this->LevelUpArmor(0.7);
    this->LevelUpDamage(.9);
};

void BrawlerClass::LevelUpSkills(int levels)
{
    Spell* new_spell;
    switch (this->master->level)
    {
        case 2:
            // new Message(Ui::msg_handler_main, MOOD_MSG, "%c%c%c%cYou've learned a new spell!%c", TCOD_COLCTRL_FORE_RGB, (int)TCODColor::blue.r, (int)TCODColor::blue.g, (int)TCODColor::blue.b, TCOD_COLCTRL_STOP);
            new_spell = new Spell;
            new_spell->master = this->master;
            new Message(Ui::msg_handler_main, MOOD_MSG, "You've learned a new spell!");
            this->master->spells->push_back(new_spell);
            new_spell->name = "Ice fist";
            new_spell->element = WaterElement;
            new_spell->attr_effect->health_current_val = -7;
            new_spell->mana_cost = 5;
            new_spell->max_range = 2;
            break;

        case 4:
            new Message(Ui::msg_handler_main, MOOD_MSG, "You've learned a new spell!");
            new_spell = new Spell;
            new_spell->master = this->master;
            this->master->spells->push_back(new_spell);
            new_spell->name = "Sacred Kick";
            new_spell->element = LifeElement;
            new_spell->attr_effect->health_current_val = -15;
            new_spell->mana_cost = 5;
            new_spell->max_range = 3;
            break;

        case 6:
            new Message(Ui::msg_handler_main, MOOD_MSG, "You've learned a new spell!");
            new_spell = new Spell;
            new_spell->master = this->master;
            this->master->spells->push_back(new_spell);
            new_spell->name = "Death's Touch";
            new_spell->element = DeathElement;
            new_spell->attr_effect->health_current_val = -25;
            new_spell->mana_cost = 20;
            new_spell->max_range = 2;
            break;

        case 8:
            new Message(Ui::msg_handler_main, MOOD_MSG, "You've learned a new spell!");
            new_spell = new Spell;
            new_spell->master = this->master;
            this->master->spells->push_back(new_spell);
            new_spell->name = "Inner Strength";
            new_spell->element = DeathElement;
            new_spell->attr_effect->damage_current_val = 15;
            new_spell->mana_cost = 20;
            new_spell->max_range = 1;
            new_spell->attr_effect->duration = 23;
            break;

    };

};
