#include "stdafx.h"
#include "class.h"
#include "actors\actor.h"
#include "attribute.h"
#include "attribute_container.h"
#include "ui.h"
#include "messages.h"
#include "spells.h"
#include "item_effect.h"
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
    switch (this->master->level)
    {
        case 2:
            // new Message(Ui::msg_handler_main, MOOD_MSG, "%c%c%c%cYou've learned a new spell!%c", TCOD_COLCTRL_FORE_RGB, (int)TCODColor::blue.r, (int)TCODColor::blue.g, (int)TCODColor::blue.b, TCOD_COLCTRL_STOP);
            new Message(Ui::msg_handler_main, MOOD_MSG, "You've learned a new technique!");
            this->master->spells->push_back(new Spell);
            this->master->spells->back()->name = "Water Bomb";
            this->master->spells->back()->element = WaterElement;
            this->master->spells->back()->spell_effect->health_current_val = -7;
            this->master->spells->back()->mana_cost = 5;
            this->master->spells->back()->max_range = 11;
            break;

        case 4:
            new Message(Ui::msg_handler_main, MOOD_MSG, "You've learned a new technique!");
            this->master->spells->push_back(new Spell);
            this->master->spells->back()->name = "AutoChemistry: Health Potion";
            this->master->spells->back()->element = LifeElement;
            this->master->spells->back()->spell_effect->health_current_val = 15;
            this->master->spells->back()->mana_cost = 5;
            this->master->spells->back()->max_range = 1;
            break;

        case 6:
            new Message(Ui::msg_handler_main, MOOD_MSG, "You've learned a new technique!");
            this->master->spells->push_back(new Spell);
            this->master->spells->back()->name = "Poison Cough";
            this->master->spells->back()->element = DeathElement;
            this->master->spells->back()->spell_effect->health_current_val = -25;
            this->master->spells->back()->mana_cost = 20;
            this->master->spells->back()->max_range = 2;
            break;

        case 8:
            new Message(Ui::msg_handler_main, MOOD_MSG, "You've learned a new technique!");
            this->master->spells->push_back(new Spell);
            this->master->spells->back()->name = "Inner Sanctuary";
            this->master->spells->back()->element = LifeElement;
            this->master->spells->back()->spell_effect->health_max_val = 30;
            this->master->spells->back()->mana_cost = 20;
            this->master->spells->back()->max_range = 1;
            this->master->spells->back()->spell_effect->duration = 50;
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
    switch (this->master->level)
    {
        case 2:
            // new Message(Ui::msg_handler_main, MOOD_MSG, "%c%c%c%cYou've learned a new spell!%c", TCOD_COLCTRL_FORE_RGB, (int)TCODColor::blue.r, (int)TCODColor::blue.g, (int)TCODColor::blue.b, TCOD_COLCTRL_STOP);
            new Message(Ui::msg_handler_main, MOOD_MSG, "You've learned a new spell!");
            this->master->spells->push_back(new Spell);
            this->master->spells->back()->name = "Ice bolt";
            this->master->spells->back()->element = WaterElement;
            this->master->spells->back()->spell_effect->health_current_val = -9;
            this->master->spells->back()->mana_cost = 7;
            this->master->spells->back()->max_range = 11;
            break;

        case 4:
            new Message(Ui::msg_handler_main, MOOD_MSG, "You've learned a new spell!");
            this->master->spells->push_back(new Spell);
            this->master->spells->back()->name = "Inner Healing Spirit";
            this->master->spells->back()->element = LifeElement;
            this->master->spells->back()->spell_effect->health_current_val = 15;
            this->master->spells->back()->mana_cost = 5;
            this->master->spells->back()->max_range = 1;
            break;

        case 6:
            new Message(Ui::msg_handler_main, MOOD_MSG, "You've learned a new spell!");
            this->master->spells->push_back(new Spell);
            this->master->spells->back()->name = "Death's Touch";
            this->master->spells->back()->element = DeathElement;
            this->master->spells->back()->spell_effect->health_current_val = -25;
            this->master->spells->back()->mana_cost = 20;
            this->master->spells->back()->max_range = 2;
            break;

        case 8:
            new Message(Ui::msg_handler_main, MOOD_MSG, "You've learned a new spell!");
            this->master->spells->push_back(new Spell);
            this->master->spells->back()->name = "Inner Fountain";
            this->master->spells->back()->element = CrystalElement;
            this->master->spells->back()->spell_effect->mana_regen_rate = 5;
            this->master->spells->back()->mana_cost = 20;
            this->master->spells->back()->max_range = 1;
            this->master->spells->back()->spell_effect->duration = 50;
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
    switch (this->master->level)
    {
        case 2:
            // new Message(Ui::msg_handler_main, MOOD_MSG, "%c%c%c%cYou've learned a new spell!%c", TCOD_COLCTRL_FORE_RGB, (int)TCODColor::blue.r, (int)TCODColor::blue.g, (int)TCODColor::blue.b, TCOD_COLCTRL_STOP);
            new Message(Ui::msg_handler_main, MOOD_MSG, "You've learned a new spell!");
            this->master->spells->push_back(new Spell);
            this->master->spells->back()->name = "Ice fist";
            this->master->spells->back()->element = WaterElement;
            this->master->spells->back()->spell_effect->health_current_val = -7;
            this->master->spells->back()->mana_cost = 5;
            this->master->spells->back()->max_range = 2;
            break;

        case 4:
            new Message(Ui::msg_handler_main, MOOD_MSG, "You've learned a new spell!");
            this->master->spells->push_back(new Spell);
            this->master->spells->back()->name = "Sacred Kick";
            this->master->spells->back()->element = LifeElement;
            this->master->spells->back()->spell_effect->health_current_val = -15;
            this->master->spells->back()->mana_cost = 5;
            this->master->spells->back()->max_range = 3;
            break;

        case 6:
            new Message(Ui::msg_handler_main, MOOD_MSG, "You've learned a new spell!");
            this->master->spells->push_back(new Spell);
            this->master->spells->back()->name = "Death's Touch";
            this->master->spells->back()->element = DeathElement;
            this->master->spells->back()->spell_effect->health_current_val = -25;
            this->master->spells->back()->mana_cost = 20;
            this->master->spells->back()->max_range = 2;
            break;

        case 8:
            new Message(Ui::msg_handler_main, MOOD_MSG, "You've learned a new spell!");
            this->master->spells->push_back(new Spell);
            this->master->spells->back()->name = "Inner Strength";
            this->master->spells->back()->element = DeathElement;
            this->master->spells->back()->spell_effect->damage_current_val = 15;
            this->master->spells->back()->mana_cost = 20;
            this->master->spells->back()->max_range = 1;
            this->master->spells->back()->spell_effect->duration = 23;
            break;

    };

};
