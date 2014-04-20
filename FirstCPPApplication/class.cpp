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


FighterClass::FighterClass()
{
    this->name = "Fighter";
};

void FighterClass::LevelUpStats(int levels)
{
    this->master->attrs->health->current_val+=3;
    this->master->attrs->health->max_val+=3;

    this->master->attrs->damage->current_val+=1;
    this->master->attrs->damage->max_val+=1;
};

void FighterClass::LevelUpSkills(int levels)
{
    switch (this->master->level)
    {
        case 2:
            // new Message(Ui::msg_handler_main, NOTYPE_MSG, "%c%c%c%cYou've learned a new spell!%c", TCOD_COLCTRL_FORE_RGB, (int)TCODColor::blue.r, (int)TCODColor::blue.g, (int)TCODColor::blue.b, TCOD_COLCTRL_STOP);
            new Message(Ui::msg_handler_main, NOTYPE_MSG, "You've learned a new technique!");
            this->master->spells->push_back(new Spell);
            this->master->spells->back()->name = "Water Bomb";
            this->master->spells->back()->element = WaterElement;
            this->master->spells->back()->spell_effect->health_current_val = -70;
            this->master->spells->back()->spell_effect->duration = 2;
            this->master->spells->back()->mana_cost = 5;
            this->master->spells->back()->max_range = 11;
            break;

        case 4:
            new Message(Ui::msg_handler_main, NOTYPE_MSG, "You've learned a new technique!");
            this->master->spells->push_back(new Spell);
            this->master->spells->back()->name = "AutoChemistry: Health Potion";
            this->master->spells->back()->element = LifeElement;
            this->master->spells->back()->spell_effect->health_current_val = -15;
            this->master->spells->back()->mana_cost = 5;
            this->master->spells->back()->max_range = 1;
            break;

        case 6:
            new Message(Ui::msg_handler_main, NOTYPE_MSG, "You've learned a new technique!");
            this->master->spells->push_back(new Spell);
            this->master->spells->back()->name = "Poison Cough";
            this->master->spells->back()->element = DeathElement;
            this->master->spells->back()->spell_effect->health_current_val = 25;
            this->master->spells->back()->mana_cost = 20;
            this->master->spells->back()->max_range = 2;
            break;

        case 8:
            break;

    };

};

MageClass::MageClass()
{
    this->name = "Mage";
};

void MageClass::LevelUpStats(int levels)
{
    this->master->attrs->health->current_val+=3;
    this->master->attrs->health->max_val+=3;

    this->master->attrs->damage->current_val+=1;
    this->master->attrs->damage->max_val+=1;
};

void MageClass::LevelUpSkills(int levels)
{
    switch (this->master->level)
    {
        case 2:
            // new Message(Ui::msg_handler_main, NOTYPE_MSG, "%c%c%c%cYou've learned a new spell!%c", TCOD_COLCTRL_FORE_RGB, (int)TCODColor::blue.r, (int)TCODColor::blue.g, (int)TCODColor::blue.b, TCOD_COLCTRL_STOP);
            new Message(Ui::msg_handler_main, NOTYPE_MSG, "You've learned a new spell!");
            this->master->spells->push_back(new Spell);
            this->master->spells->back()->name = "Ice bolt";
            this->master->spells->back()->element = WaterElement;
            this->master->spells->back()->spell_effect->health_current_val = 7;
            this->master->spells->back()->mana_cost = 5;
            this->master->spells->back()->max_range = 11;
            break;

        case 4:
            new Message(Ui::msg_handler_main, NOTYPE_MSG, "You've learned a new spell!");
            this->master->spells->push_back(new Spell);
            this->master->spells->back()->name = "Sacred Love";
            this->master->spells->back()->element = LifeElement;
            this->master->spells->back()->spell_effect->health_current_val = -15;
            this->master->spells->back()->mana_cost = 5;
            this->master->spells->back()->max_range = 1;
            break;

        case 6:
            new Message(Ui::msg_handler_main, NOTYPE_MSG, "You've learned a new spell!");
            this->master->spells->push_back(new Spell);
            this->master->spells->back()->name = "Death's Touch";
            this->master->spells->back()->element = DeathElement;
            this->master->spells->back()->spell_effect->health_current_val = 25;
            this->master->spells->back()->mana_cost = 20;
            this->master->spells->back()->max_range = 2;
            break;

        case 8:
            break;

    };

};

BrawlerClass::BrawlerClass()
{
    this->name = "Brawler";
};

void BrawlerClass::LevelUpStats(int levels)
{
    this->master->attrs->health->current_val+=3;
    this->master->attrs->health->max_val+=3;

    this->master->attrs->damage->current_val+=1;
    this->master->attrs->damage->max_val+=1;
};

void BrawlerClass::LevelUpSkills(int levels)
{
    switch (this->master->level)
    {
        case 2:
            // new Message(Ui::msg_handler_main, NOTYPE_MSG, "%c%c%c%cYou've learned a new spell!%c", TCOD_COLCTRL_FORE_RGB, (int)TCODColor::blue.r, (int)TCODColor::blue.g, (int)TCODColor::blue.b, TCOD_COLCTRL_STOP);
            new Message(Ui::msg_handler_main, NOTYPE_MSG, "You've learned a new spell!");
            this->master->spells->push_back(new Spell);
            this->master->spells->back()->name = "Ice fist";
            this->master->spells->back()->element = WaterElement;
            this->master->spells->back()->spell_effect->health_current_val = 7;
            this->master->spells->back()->mana_cost = 5;
            this->master->spells->back()->max_range = 1;
            break;

        case 4:
            new Message(Ui::msg_handler_main, NOTYPE_MSG, "You've learned a new spell!");
            this->master->spells->push_back(new Spell);
            this->master->spells->back()->name = "Sacred Kick";
            this->master->spells->back()->element = LifeElement;
            this->master->spells->back()->spell_effect->health_current_val = -15;
            this->master->spells->back()->mana_cost = 5;
            this->master->spells->back()->max_range = 1;
            break;

        case 6:
            new Message(Ui::msg_handler_main, NOTYPE_MSG, "You've learned a new spell!");
            this->master->spells->push_back(new Spell);
            this->master->spells->back()->name = "Death's Touch";
            this->master->spells->back()->element = DeathElement;
            this->master->spells->back()->spell_effect->health_current_val = 25;
            this->master->spells->back()->mana_cost = 20;
            this->master->spells->back()->max_range = 2;
            break;

        case 8:
            break;

    };

};
