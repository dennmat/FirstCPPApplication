#include "stdafx.h"
#include <iostream>
#include "ui.h"

#include "game.h"
#include "person.h"
#include "utils.h"
#include "inventory.h"
#include "equipment.h"
#include "combat.h"
#include "attribute_container.h"
#include "attribute.h"

Ui::Ui()
{
    this->turn_checking_against = 1;
    this->last_turn_noted = 1;

    this->tick_turn_changed = 0;

    this->tick_threshold = 50;

    this->tick_checking_against = this->tick_threshold;
};

void Ui::update_ui()
{
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

            unsigned long long int difference = this->tick_checking_against - this->game->tick_count ;
            if (difference >= 0)
            {
                if (difference > this->tick_threshold)
                {
                    this->last_turn_noted = this->game->turn_count;
                    this->tick_turn_changed = this->game->tick_count;
                };
                float coef = (float)difference / (float)(this->tick_threshold);
                TCODColor myColor = TCODColor::lerp ( TCODColor::white, TCODColor::red, coef );
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

    //player stats
    ui_sidebar_con->print(0, 2, "PLAYER NAME %s", this->game->player->GetNameC());
    ui_sidebar_con->print(0, 3, "PLAYER HP %d", this->game->player->attrs->health->current_val);

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
