#include "stdafx.h"
#include <iostream>
#include <vector>
#include "ui.h"
//#include <cmath>

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
#include "messages.h"

// MessageHandler* Ui::msg_handler_main = new MessageHandler;
// Item* Ui::chosen_item = NULL;
    Game* Ui::game = game;

    Item* Ui::chosen_item = NULL;

    bool Ui::item_active = false;

    unsigned long long int Ui::turn_checking_against = 1;
    unsigned long long int Ui::last_turn_noted = 1;
    unsigned long long int Ui::tick_turn_changed = 0;
    unsigned long long int Ui::tick_threshold = 50;
    unsigned long long int Ui::tick_checking_against = Ui::tick_threshold;

        int Ui::ui_inv_w = 0;
        int Ui::ui_inv_h = 0;
        int Ui::ui_inv_msg_w = 0;
        int Ui::ui_inv_msg_h = 0;

        int Ui::ui_msg_w = 0;
        int Ui::ui_msg_h = 0;
        int Ui::ui_sidebar_w = 0;
        int Ui::ui_sidebar_h = 0;

    MessageHandler* Ui::msg_handler_main = new MessageHandler;
//  Ui::Ui()
// {
//     Ui::chosen_item = NULL;
//     Ui::item_active = false;
// 
//     Ui::turn_checking_against = 1;
//     Ui::last_turn_noted = 1;
// 
//     Ui::tick_turn_changed = 0;
// 
//     Ui::tick_threshold = 50;
// 
//     Ui::tick_checking_against = Ui::tick_threshold;
//     Ui::msg_handler_main = new MessageHandler;
//     Ui::msg_handler_main->game = Ui::game;
// };
// 
// Ui::Ui(Game* game)
// {
//     Ui::chosen_item = NULL;
//     Ui::item_active = false;
// 
//     Ui::turn_checking_against = 1;
//     Ui::last_turn_noted = 1;
// 
//     Ui::tick_turn_changed = 0;
// 
//     Ui::tick_threshold = 50;
// 
//     Ui::tick_checking_against = Ui::tick_threshold;
//     Ui::msg_handler_main = new MessageHandler;
// 
//     Ui::game = game;
//     Ui::msg_handler_main->game = Ui::game;
// };

void Ui::update_inventory_ui()
{

};

void Ui::update_ui()
{


    // all of this is to update the turn counter
    TCODConsole::setColorControl(TCOD_COLCTRL_1, TCODColor::white, TCODColor::black);

    if (Ui::turn_checking_against < Ui::game->turn_count)
    {
        //reset the future time
        Ui::tick_checking_against = Ui::game->tick_count+Ui::tick_threshold;
        Ui::turn_checking_against = Ui::game->turn_count;

    };
    if (Ui::last_turn_noted != Ui::turn_checking_against)
    {
        //if turn was changed less than 300 ticks ago, make the turn count red
        if (Ui::game->tick_count < Ui::tick_checking_against)
        {
            unsigned long long int difference = Ui::tick_checking_against - Ui::game->tick_count;
            if (difference >= 0)
            {
                if (difference > Ui::tick_threshold)
                {
                    Ui::last_turn_noted = Ui::game->turn_count;
                    Ui::tick_turn_changed = Ui::game->tick_count;
                };
                float coef = (float)difference / (float)(Ui::tick_threshold);
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
    ui_sidebar_h = Ui::game->screen_h-ui_msg_h;
    TCODConsole *ui_sidebar_con = new TCODConsole(ui_sidebar_w, ui_sidebar_h);

    //reset ui console to default
    TCODColor ui_sidebar_color(10, 5, 5);
    TCODColor ui_sidebar_fore = ui_sidebar_con->getDefaultForeground();
    ui_sidebar_con->setDefaultBackground(ui_sidebar_color);
    ui_sidebar_con->clear();


    //draw the message text
    ui_sidebar_con->print(0, 0, "TURN COUNT %c%d%c", TCOD_COLCTRL_1, Ui::game->turn_count, TCOD_COLCTRL_STOP);

    //  player stats
    //generate a color for the percent of players' cur hp to max hp between red and green
    float player_hp_percent = (float)Ui::game->player->attrs->health->current_val / (float)Ui::game->player->attrs->health->max_val;
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

    ui_sidebar_con->print(0, 2, "PLAYER NAME %s", Ui::game->player->GetNameC());
    ui_sidebar_con->print(0, 3, "PLAYER HP %c%d%c", TCOD_COLCTRL_1, Ui::game->player->attrs->health->current_val, TCOD_COLCTRL_STOP);

    //mouse stats
    ui_sidebar_con->print(0, 5, "MOUSE X Y" );
    ui_sidebar_con->print(0, 6, "%d %d", Ui::game->mouse_evt.cx, Ui::game->mouse_evt.cy);

    //is moving in a direciton
    const char* move_left;
    const char* move_right;
    move_left = BoolToString(Ui::game->player->is_moving_left, false);
    move_right = BoolToString(Ui::game->player->is_moving_right, false);

    ui_sidebar_con->print(1, 8, "%s", BoolToString(Ui::game->player->is_moving_up, false) );
    ui_sidebar_con->print(0, 9, "%s %s", move_left, move_right);
    ui_sidebar_con->print(1, 10, "%s", BoolToString(Ui::game->player->is_moving_down, false));

    //draw player inventory info
    ui_sidebar_con->print(0, 12, "Items in inventory:");
    ui_sidebar_con->print(0, 13, "%d", Ui::game->player->inventory->get_count());

    int initial_y = 15;
    int y = initial_y;
    int ci = 0;
    TCODColor attr_colors[4] = {TCODColor::lightGreen, TCODColor::lightBlue, TCODColor::darkerGrey, TCODColor::lightRed};
    std::vector<std::string> player_attrs = Ui::game->player->attrs->PrettyVector();
    for (std::vector<std::string>::iterator it = player_attrs.begin(); it !=player_attrs.end(); ++it)
    {
        ui_sidebar_con->setDefaultForeground(attr_colors[ci]);
        ui_sidebar_con->print(0, y, "%s", (it->c_str()));
        ++y;

        //add a space between the 4 types of attrs
        if ((y - (initial_y-1)) % 5 == 0)
        {
            ui_sidebar_con->print(0, y, " ");
            y++;
            ci++;
        }

    };

    y++;

    // experience bar
    ui_sidebar_con->setDefaultForeground(ui_sidebar_fore);

    std::string left_exp = ">";
    std::string right_exp = "";
    float left_percent = ((float)(Ui::game->player->xp_this_level)) / Ui::game->player->xp_required ;
    if (left_percent < 0) left_percent = 0.0f;

    float right_raw = ((float)Ui::game->player->xp_required - (float)Ui::game->player->xp_this_level);
    float right_percent = right_raw / (float)Ui::game->player->xp_required;
    if (right_percent < 0) right_percent = 0.0f;

    int left_count = floor(left_percent * (float)8);
    int right_count = floor(right_percent * (float)8);
    if (left_count + right_count < 8) right_count++;

    left_exp.append(left_count, '-');
    right_exp.append(right_count, '-');
    right_exp.append(1, '>');

    ui_sidebar_con->print(0, y, "%c%s%c%s", TCOD_COLCTRL_1, left_exp.c_str(), TCOD_COLCTRL_STOP, right_exp.c_str());


    //draw ui console to root
    TCODConsole::blit(ui_sidebar_con, 0, 0, ui_sidebar_w, ui_sidebar_h, TCODConsole::root, Ui::game->screen_w-ui_sidebar_w, 0 );
    delete ui_sidebar_con;
};

void Ui::draw_ui_msg()
{
    ui_msg_w = Ui::game->screen_w;
    ui_msg_h = 10;
    TCODConsole *ui_msg_con = new TCODConsole(ui_msg_w, ui_msg_h);

    //reset ui console to default
    TCODColor ui_msg_color(12,12,12);
    ui_msg_con->setDefaultBackground(ui_msg_color);
    ui_msg_con->clear();

    //draw the message text
    Ui::msg_handler_main->draw_raw(ui_msg_con);

    //draw ui console to root
    TCODConsole::blit(ui_msg_con, 0, 0, ui_msg_w, ui_msg_h, TCODConsole::root, 0, Ui::game->screen_h-ui_msg_h);
    delete ui_msg_con;
};


void Ui::draw_inventory_main()
{
    Ui::draw_inventory_ui();
    Ui::draw_inventory_msg();
};

void Ui::draw_inventory_ui()
{
    // clear the screen
    TCODConsole::root->clear();

    ui_inv_w = Ui::game->screen_w;
    ui_inv_h = Ui::game->screen_h - 10;
    TCODConsole *ui_inv_con = new TCODConsole(ui_inv_w, ui_inv_h);

    int inv_title_x = Ui::game->screen_w/2;
    TCOD_bkgnd_flag_t bkgnd_flag = ui_inv_con->getBackgroundFlag();
    ui_inv_con->printEx(inv_title_x, 2, bkgnd_flag, TCOD_alignment_t::TCOD_CENTER, "Inventory Screen");

    // draw the list of items on the player
    std::vector<Item*>* v  = Ui::game->player->inventory->items;
    int i = 5;
    char key = 'a';
    bool is_chosen, is_active;
    for(std::vector<Item*>::iterator it = v->begin(); it != v->end(); ++it) 
    {
        is_chosen = (*it) == Ui::chosen_item;
        is_active = Ui::item_active;

        //TODO: Clean the shit out of this whole thing
        if (Ui::game->player->equipment->is_item_equipped(*it))
        {
            if (is_chosen)
            {
                if (is_active)
                {
                    TCODConsole::setColorControl(TCOD_COLCTRL_1, TCODColor::red+TCODColor::blue, TCODColor::black);
                }
                else 
                {
                TCODConsole::setColorControl(TCOD_COLCTRL_1, TCODColor::red+TCODColor::green, TCODColor::black);
                }
            }
            else
            {
                TCODConsole::setColorControl(TCOD_COLCTRL_1, TCODColor::red, TCODColor::black);
            };
            ui_inv_con->print(3, i, "%c- %c%s%c", key, TCOD_COLCTRL_1, (*it)->name.c_str(), TCOD_COLCTRL_STOP);
        }
        else
        {
            if (is_chosen)
            {
                if (is_active)
                {
                    TCODConsole::setColorControl(TCOD_COLCTRL_1, TCODColor::red+TCODColor::yellow, TCODColor::black);
                }
                else
                {
                TCODConsole::setColorControl(TCOD_COLCTRL_1, TCODColor::green, TCODColor::black);
                }
            }
            else
            {
                TCODConsole::setColorControl(TCOD_COLCTRL_1, TCODColor::white, TCODColor::black);
            };
            ui_inv_con->print(3, i, "%c- %c%s%c", key, TCOD_COLCTRL_1, (*it)->name.c_str(), TCOD_COLCTRL_STOP);
        };
        i++;

        ui_inv_con->print(3, i, (*it)->item_effect->oneline_str().c_str());
        i++;

        key++;

    }

    TCODConsole::blit(ui_inv_con, 0, 0, ui_inv_w, ui_inv_h, TCODConsole::root, 0, 0);
    delete ui_inv_con;

};

void Ui::draw_inventory_msg()
{
    ui_inv_msg_w = Ui::game->screen_w;
    ui_inv_msg_h = 10;
    TCODConsole *ui_inv_msg_con = new TCODConsole(ui_inv_msg_w, ui_inv_msg_h);

    //reset ui console to default
    TCODColor ui_inv_msg_color(12,12,12);
    ui_inv_msg_con->setDefaultBackground(ui_inv_msg_color);
    ui_inv_msg_con->clear();

    //draw the message text
    ui_inv_msg_con->print(0, 0, "is item chosen? %i", Ui::item_is_chosen());
    ui_inv_msg_con->print(0, 1, "is item active? %i", Ui::item_active);

    //draw ui console to root
    TCODConsole::blit(ui_inv_msg_con, 0, 0, ui_inv_msg_w, ui_inv_msg_h, TCODConsole::root, 0, Ui::game->screen_h-ui_inv_msg_h);
    delete ui_inv_msg_con;
};

bool Ui::item_is_chosen()
{
    return Ui::chosen_item != NULL;
};
