#include "stdafx.h"
#include <iostream>
#include <vector>
#include "ui.h"

#include "game.h"
#include <actors\Person.h>
#include "utils.h"
#include "inventory.h"
#include "equipment.h"
#include "combat.h"
#include "attribute_container.h"
#include "attribute.h"
#include "item.h"
#include "item_effect.h"

Ui::Ui()
{
    this->chosen_item = NULL;
    this->item_active = false;

    this->turn_checking_against = 1;
    this->last_turn_noted = 1;

    this->tick_turn_changed = 0;

    this->tick_threshold = 50;

    this->tick_checking_against = this->tick_threshold;
};

void Ui::update_inventory_ui()
{

};

void Ui::update_ui()
{
    TCODConsole::setColorControl(TCOD_COLCTRL_1, TCODColor::white, TCODColor::black);

    if (this->turn_checking_against < this->game->turn_count)
    {
        //reset the future time
        this->tick_checking_against = this->game->tick_count+this->tick_threshold;
        this->turn_checking_against = this->game->turn_count;

    };
    if (this->last_turn_noted != this->turn_checking_against)
    {
        //if turn was changed less than 300 ticks ago, make the turn count red
        if (this->game->tick_count < this->tick_checking_against)
        {
            unsigned long long int difference = this->tick_checking_against - this->game->tick_count;
            if (difference >= 0)
            {
                if (difference > this->tick_threshold)
                {
                    this->last_turn_noted = this->game->turn_count;
                    this->tick_turn_changed = this->game->tick_count;
                };
                float coef = (float)difference / (float)(this->tick_threshold);
                TCODColor myColor = TCODColor::lerp ( TCODColor::white, TCODColor(93,93,93), coef );
                TCODConsole::setColorControl(TCOD_COLCTRL_1, myColor, TCODColor::black);
            }
            else
            {
                TCODConsole::setColorControl(TCOD_COLCTRL_1, TCODColor::white, TCODColor::black);
            };
        }
        //otherwise make it yellow because enough time has passed and reset the
        //the counters to update that we're finally in the new time
        else
        {
            TCODConsole::setColorControl(TCOD_COLCTRL_1, TCODColor::white, TCODColor::black);
        };
    }
}

void Ui::draw_ui()
{
    draw_ui_msg();
    draw_ui_sidebar();
};

void Ui::draw_ui_sidebar()
{
    ui_sidebar_w = 20;
    ui_sidebar_h = this->game->screen_h-ui_msg_h;
    TCODConsole *ui_sidebar_con = new TCODConsole(ui_sidebar_w, ui_sidebar_h);

    //reset ui console to default
    TCODColor ui_sidebar_color(10, 5, 5);
    ui_sidebar_con->setDefaultBackground(ui_sidebar_color);
    ui_sidebar_con->clear();


    //draw the message text
    ui_sidebar_con->print(0, 0, "TURN COUNT %c%d%c", TCOD_COLCTRL_1, this->game->turn_count, TCOD_COLCTRL_STOP);

    //  player stats
    //generate a color for the percent of players' cur hp to max hp between red and green
    float player_hp_percent = (float)this->game->player->attrs->health->current_val / (float)this->game->player->attrs->health->max_val;
    TCODColor player_hp_color = TCODColor::lerp ( TCODColor::red, TCODColor::green, player_hp_percent);
    if (player_hp_percent >= 1.0)
    {
        TCODConsole::setColorControl(TCOD_COLCTRL_1, TCODColor::green, TCODColor::black);
    }
    else if (player_hp_percent <= 0)
    {
        TCODConsole::setColorControl(TCOD_COLCTRL_1, TCODColor::red, TCODColor::black);
    }
    else
    {
        TCODConsole::setColorControl(TCOD_COLCTRL_1, player_hp_color, TCODColor::black);
    };

    ui_sidebar_con->print(0, 2, "PLAYER NAME %s", this->game->player->GetNameC());
    ui_sidebar_con->print(0, 3, "PLAYER HP %c%d%c", TCOD_COLCTRL_1, this->game->player->attrs->health->current_val, TCOD_COLCTRL_STOP);

    //mouse stats
    ui_sidebar_con->print(0, 5, "MOUSE X Y" );
    ui_sidebar_con->print(0, 6, "%d %d", this->game->mouse_evt.cx, this->game->mouse_evt.cy);

    //is moving in a direciton
    const char* move_left;
    const char* move_right;
    move_left = BoolToString(this->game->player->is_moving_left, false);
    move_right = BoolToString(this->game->player->is_moving_right, false);

    ui_sidebar_con->print(1, 8, "%s", BoolToString(this->game->player->is_moving_up, false) );
    ui_sidebar_con->print(0, 9, "%s %s", move_left, move_right);
    ui_sidebar_con->print(1, 10, "%s", BoolToString(this->game->player->is_moving_down, false));

    //draw player inventory info
    ui_sidebar_con->print(0, 13, "%d", this->game->player->inventory->get_count());

    //draw ui console to root
    TCODConsole::blit(ui_sidebar_con, 0, 0, ui_sidebar_w, ui_sidebar_h, TCODConsole::root, this->game->screen_w-ui_sidebar_w, 0 );
    delete ui_sidebar_con;
};

void Ui::draw_ui_msg()
{
    ui_msg_w = this->game->screen_w;
    ui_msg_h = 10;
    TCODConsole *ui_msg_con = new TCODConsole(ui_msg_w, ui_msg_h);

    //reset ui console to default
    TCODColor ui_msg_color(12,12,12);
    ui_msg_con->setDefaultBackground(ui_msg_color);
    ui_msg_con->clear();

    //draw the message text
    ui_msg_con->print(0, 0, "TURN COUNT %d", this->game->turn_count);

    //draw ui console to root
    TCODConsole::blit(ui_msg_con, 0, 0, ui_msg_w, ui_msg_h, TCODConsole::root, 0, this->game->screen_h-ui_msg_h);
    delete ui_msg_con;
};


void Ui::draw_inventory_main()
{
    this->draw_inventory_ui();
    this->draw_inventory_msg();
};

void Ui::draw_inventory_ui()
{
    // clear the screen
    TCODConsole::root->clear();

    ui_inv_w = this->game->screen_w;
    ui_inv_h = this->game->screen_h - 10;
    TCODConsole *ui_inv_con = new TCODConsole(ui_inv_w, ui_inv_h);

    int inv_title_x = this->game->screen_w/2;
    TCOD_bkgnd_flag_t bkgnd_flag = ui_inv_con->getBackgroundFlag();
    ui_inv_con->printEx(inv_title_x, 2, bkgnd_flag, TCOD_alignment_t::TCOD_CENTER, "Inventory Screen");

    // draw the list of items on the player
    std::vector<Item*>* v  = this->game->player->inventory->items;
    int i = 5;
    bool is_chosen;
    for(std::vector<Item*>::iterator it = v->begin(); it != v->end(); ++it) 
    {
        is_chosen = (*it) == this->chosen_item;
        if (this->game->player->equipment->is_item_equipped(*it))
        {
            if (is_chosen)
            {
                TCODConsole::setColorControl(TCOD_COLCTRL_1, TCODColor::green, TCODColor::black);
            }
            else
            {
                TCODConsole::setColorControl(TCOD_COLCTRL_1, TCODColor::red, TCODColor::black);
            };
            ui_inv_con->print(3, i, "%c%s%c", TCOD_COLCTRL_1, (*it)->name.c_str(), TCOD_COLCTRL_STOP);
        }
        else
        {
            if (is_chosen)
            {
                TCODConsole::setColorControl(TCOD_COLCTRL_1, TCODColor::green, TCODColor::black);
            }
            else
            {
                TCODConsole::setColorControl(TCOD_COLCTRL_1, TCODColor::white, TCODColor::black);
            };
            ui_inv_con->print(3, i, "%c%s%c", TCOD_COLCTRL_1, (*it)->name.c_str(), TCOD_COLCTRL_STOP);
        };
        i++;

        ui_inv_con->print(3, i, (*it)->item_effect->oneline_str().c_str());
        i++;
    }

    TCODConsole::blit(ui_inv_con, 0, 0, ui_inv_w, ui_inv_h, TCODConsole::root, 0, 0);
    delete ui_inv_con;

};

void Ui::draw_inventory_msg()
{
    ui_inv_msg_w = this->game->screen_w;
    ui_inv_msg_h = 10;
    TCODConsole *ui_inv_msg_con = new TCODConsole(ui_inv_msg_w, ui_inv_msg_h);

    //reset ui console to default
    TCODColor ui_inv_msg_color(12,12,12);
    ui_inv_msg_con->setDefaultBackground(ui_inv_msg_color);
    ui_inv_msg_con->clear();

    //draw the message text
    ui_inv_msg_con->print(0, 0, "is item chosen? %i", this->game->ui->item_is_chosen());
    ui_inv_msg_con->print(0, 1, "is item active? %i", this->game->ui->item_active);

    //draw ui console to root
    TCODConsole::blit(ui_inv_msg_con, 0, 0, ui_inv_msg_w, ui_inv_msg_h, TCODConsole::root, 0, this->game->screen_h-ui_inv_msg_h);
    delete ui_inv_msg_con;
};

bool Ui::item_is_chosen()
{
    return this->chosen_item != NULL;
};
