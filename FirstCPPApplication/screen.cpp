#include "stdafx.h"
#include "screen.h"
#include <game.h>
#include <ui.h>
#include "input.h"
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
#include "map.h"
#include "tile.h"
#include "draw_utils.h"
#include "Representation.h"
#include "spells.h"
#include "class.h"
#include "enums\hunger_threshold.h"
#include <attribute_container.h>
#include <attribute.h>


template<typename T>
Screen<T>::Screen() 
{
    this->title = "Untitled Screen";

    this->offset = 5;
    this->key = 'a';
    this->hline = 2;

    this->elements = new std::vector<T*>;
};
template Screen<Item>::Screen();

// InventoryScreen::InventoryScreen() : Screen()
// {
// };

template<typename T>
void Screen<T>::draw_screen_title(TCODConsole* con)
{
    int inv_title_x = Ui::game->screen_w/2;
    TCOD_bkgnd_flag_t bkgnd_flag = con->getBackgroundFlag();
    con->printEx(inv_title_x, 2, bkgnd_flag, TCOD_alignment_t::TCOD_CENTER, title.c_str());
};


template<typename T>
void Screen<T>::draw_mouse_horiz_line(TCODConsole* con)
{
    //draw mouse line
    con->hline(0, Game::mouse_evt.cy, this->hline);
    con->putChar(this->hline, Game::mouse_evt.cy, '>');

};
template void Screen<Item>::draw_mouse_horiz_line(TCODConsole* con);

template<typename T>
TCODConsole* Screen<T>::create_screen()
{
    int con_w = Game::screen_w;
    int con_h = Game::screen_h - 10;
    TCODConsole *con = new TCODConsole(con_w, con_h);
    return con;
};
template TCODConsole* Screen<Item>::create_screen();

template<typename T>
void Screen<T>::draw()
{

    TCODConsole::root->clear();

    TCODConsole *con = this->create_screen();
    this->draw_screen_title(con);

    //draw mouse line
    this->draw_mouse_horiz_line(con);

    //     if (loop_through_lines != NULL)
    //     {
    //         loop_through_lines(con, offset, i, key);
    int i = 5;
    this->loop(con, i);
    //     };

    TCODConsole::blit(con, 0, 0, Ui::ui_inv_w, Ui::ui_inv_h, TCODConsole::root, 0, 0);
    delete con;

};
template void Screen<Item>::draw();
// void Screen::loop(TCODConsole* con, int i)
// {
// };

template<typename T>
void InventoryScreen<T>::loop(TCODConsole* con, int i)
{
    TCODColor foreground, background;
    foreground = TCODColor::white;

    bool is_chosen, is_active;
    std::vector<Item*>* v  = Ui::game->player->inventory->items;
    int num_items = v->size();

    if (Ui::offset >= num_items && num_items > 0) //if the offset is passed the end of the list
    {
        Ui::page_num = std::ceil(((float)num_items/Ui::per_page))-1; //round up and then take off a page
        Ui::offset = Ui::page_num*Ui::per_page;
    };

    this->build_screen_items(con, i);
};
template void InventoryScreen<Item>::loop(TCODConsole* con, int i);

template<typename T>
void InventoryScreen<T>::build_screen_items(TCODConsole* con, int i)
{
    bool is_chosen, is_active;
    TCODColor foreground, background;
    std::vector<Item*>::iterator it = this->elements->begin() + Ui::offset;
    for (it; it != this->elements->end() && it - this->elements->begin() != (Ui::offset + Ui::per_page); ++it) 
    {
        std::string msg_str = "%c-%c%c%c %c%s%c : %cweighs %d stones%c";
        is_chosen = (*it) == Ui::chosen_generic;
        is_active = Ui::generic_active;

        TCODConsole::setColorControl(TCOD_COLCTRL_2, *(*it)->repr->fg_color, con->getDefaultBackground());
        if (Ui::game->player->equipment->is_item_equipped(*it))
        {
            background = TCODColor::darkestRed;
            if (is_chosen)
            {
                msg_str.append(" <-");
                // background = TCODColor::white;
                if (is_active) { foreground = TCODColor::red+TCODColor::blue; }
                else { foreground = TCODColor::red+TCODColor::green; }
            }
            else { foreground = TCODColor::red;
            };
        }
        else
        {
            background = con->getDefaultBackground();
            if (is_chosen)
            {
                msg_str.append(" <-");
                if (is_active) { foreground = TCODColor::red+TCODColor::yellow; }
            }
            else
            {
                foreground = TCODColor::white;
            };
        };

        //mouse selection
        if (Game::mouse_evt.lbutton_pressed)
        {
            if (Game::mouse_evt.cy == i)
            {
                if ( (*it)!= Ui::chosen_generic)
                {
                    Ui::chosen_generic = (*it);
                    Ui::generic_active = false;
                }
                else if ( (*it) == Ui::chosen_generic)
                {
                    Ui::generic_active = true;
                    background = TCODColor::green;
                };
            }
        }
        else if (Game::mouse_evt.rbutton_pressed)
        {
            Ui::chosen_generic = NULL;
            Ui::generic_active = false;
        };

        //print the item name and selection
        TCODConsole::setColorControl(TCOD_COLCTRL_1, foreground, background);
        TCODConsole::setColorControl(TCOD_COLCTRL_3, TCODColor::lightGrey, background);
        const char *msg_char = msg_str.c_str();
        con->printEx(offset, i, TCOD_bkgnd_flag_t::TCOD_BKGND_SET,
                TCOD_alignment_t::TCOD_LEFT, msg_char, key, TCOD_COLCTRL_2,
                (*it)->repr->repr, TCOD_COLCTRL_STOP, TCOD_COLCTRL_1,
                (*it)->name.c_str(), TCOD_COLCTRL_STOP, TCOD_COLCTRL_3, (*it)->weight, TCOD_COLCTRL_STOP);
        i++;

        //print the item effects
        std::string msg = (*it)->item_effect->oneline_str();
        std::vector<TCOD_colctrl_t> colctrl_vec = (*it)->item_effect->oneline_str_colours();
        one_line_helper(con, offset, i, msg, colctrl_vec);
        i++;
        i++;

        key++;

    }

};
template void InventoryScreen<Item>::build_screen_items(TCODConsole* con, int i);
