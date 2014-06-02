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
#include "screenitem.h"


    template<typename T>
Screen<T>::Screen() 
{
    this->title = "Untitled Screen";

    this->offset = 5;
    this->key = 'a';
    this->hline = 2;

    this->elements = new std::vector<T*>;
    this->screen_items = new std::vector<ScreenItem*>;
};
template Screen<Item>::Screen();
template Screen<Spell>::Screen();
template Screen<IClass>::Screen();

    template<typename T>
bool Screen<T>::is_chosen(T* element)
{
    return element == Ui::chosen_generic;
};
template bool Screen<Item>::is_chosen(Item* element);
template bool Screen<Spell>::is_chosen(Spell* element);
template bool Screen<IClass>::is_chosen(IClass* element);

    template<typename T>
bool Screen<T>::is_active(T* element)
{
    return Ui::generic_active;
};
template bool Screen<Item>::is_active(Item* element);
template bool Screen<Spell>::is_active(Spell* element);
template bool Screen<IClass>::is_active(IClass* element);

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
template void Screen<IClass>::draw();
template void Screen<Spell>::draw();

    template<typename T>
void Screen<T>::build_screen_items(TCODConsole* con, int i)
{
    std::vector<T*>::iterator it = this->elements->begin() + Ui::offset;
    for (it; it != this->elements->end() && it - this->elements->begin() != (Ui::offset + Ui::per_page); ++it) 
    {
        ScreenItem si = this->build_screen_item(con, i, *it);
        this->draw_screen_item(con, i, si);
        si.handle_mouse(i);

        this->key++;

    }

};
template void Screen<Item>::build_screen_items(TCODConsole* con, int i);
template void Screen<IClass>::build_screen_items(TCODConsole* con, int i);
template void Screen<Spell>::build_screen_items(TCODConsole* con, int i);


    template<typename T>
void Screen<T>::loop(TCODConsole* con, int i)
{
    TCODColor foreground, background;
    foreground = TCODColor::white;

    bool is_chosen, is_active;
    std::vector<T*>* v  = this->elements;
    int num_elems = v->size();

    if (Ui::offset >= num_elems && num_elems > 0) //if the offset is passed the end of the list
    {
        Ui::page_num = std::ceil(((float)num_elems/(float)Ui::per_page))-1; //round up and then take off a page
        Ui::offset = Ui::page_num*Ui::per_page;
    };

    this->build_screen_items(con, i);
};
template void Screen<Item>::loop(TCODConsole* con, int i);
template void Screen<IClass>::loop(TCODConsole* con, int i);
template void Screen<Spell>::loop(TCODConsole* con, int i);

template<typename T>
std::vector<TCODColor> Screen<T>::get_colors(TCODConsole* con, T* element)
{
    TCODColor foreground, background;
    if (this->is_chosen(element))
    {
        if (this->is_active(element))
        { 
            foreground = TCODColor::red+TCODColor::yellow; 
        }
    }
    else
    {
        foreground = TCODColor::white;
    };

    background = con->getDefaultBackground();

    std::vector<TCODColor> result;
    result.push_back(foreground);
    result.push_back(background);
    return result;


};

    template<typename T>
bool InventoryScreen<T>::is_enabled(T* element)
{
    return Ui::game->player->equipment->is_item_equipped(element);
};
template bool InventoryScreen<Item>::is_enabled(Item* element);


    template<typename T>
ScreenItem InventoryScreen<T>::build_screen_item(TCODConsole* con, int i, T* element)
{
    ScreenItem result;
    TCODColor foreground, background;
    std::string msg_str = "%c-%c%c%c %c%s%c : %cweighs %d stones%c";

    bool is_chosen = this->is_chosen(element);
    bool is_active = this->is_active(element);
    bool is_enabled = this->is_enabled(element);

        if (is_chosen)
        {
            msg_str.append(" <-");
            if (is_active) { foreground = TCODColor::red+TCODColor::blue; }
            else { foreground = TCODColor::red+TCODColor::green; }
        }

        std::vector<TCODColor> colors = this->get_colors(con,  element);
        foreground = colors.at(0);
        background = colors.at(1);

    char buffer[512];
    TCODConsole::setColorControl(TCOD_COLCTRL_2, *(element)->repr->fg_color, con->getDefaultBackground());
    TCODConsole::setColorControl(TCOD_COLCTRL_3, TCODColor::lightGrey, background);
    sprintf(buffer, msg_str.c_str(), this->key, TCOD_COLCTRL_2,
            (element)->repr->repr, TCOD_COLCTRL_STOP, TCOD_COLCTRL_1,
            (element)->name.c_str(), TCOD_COLCTRL_STOP, TCOD_COLCTRL_3, (element)->weight, TCOD_COLCTRL_STOP);
    msg_str = buffer;
    result.foreground = foreground;
    result.background = background;
    result.msg_str = msg_str;
    result.element = element;

    return result;
};
template ScreenItem InventoryScreen<Item>::build_screen_item(TCODConsole* con, int i, Item* element);

template<typename T>
std::vector<TCODColor> InventoryScreen<T>::get_colors(TCODConsole* con, T* element)
{
    TCODColor foreground, background;


    bool is_chosen = this->is_chosen(element);
    bool is_active = this->is_active(element);
    bool is_enabled = this->is_enabled(element);

    if (is_enabled)
    {
        background = TCODColor::darkestRed;
        if (is_chosen)
        {
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
            if (is_active) { foreground = TCODColor::red+TCODColor::yellow; }
        }
        else
        {
            foreground = TCODColor::white;
        };
    };

    std::vector<TCODColor> result;
    result.push_back(foreground);
    result.push_back(background);
    return result;
};
template std::vector<TCODColor> InventoryScreen<Item>::get_colors(TCODConsole* con, Item* element);

    template<typename T>
void InventoryScreen<T>::draw_screen_item(TCODConsole* con, int& i, ScreenItem& si)
{
    //print the item name and selection
    TCODConsole::setColorControl(TCOD_COLCTRL_1, si.foreground, si.background);
    const char *msg_char = si.msg_str.c_str();
    si.min_y = i;
    con->printEx(this->offset, i, TCOD_bkgnd_flag_t::TCOD_BKGND_SET,
            TCOD_alignment_t::TCOD_LEFT, msg_char);

    i++;

    //print the item effects
    std::string msg = ((T*)si.element)->item_effect->oneline_str();
    std::vector<TCOD_colctrl_t> colctrl_vec = ((T*)si.element)->item_effect->oneline_str_colours();
    one_line_helper(con, this->offset, i, msg, colctrl_vec);
    si.max_y = i;
    // printf("setting min %d max %d\n", si.min_y, si.max_y);
    i++;
    i++;
};
template void InventoryScreen<Item>::draw_screen_item(TCODConsole* con, int& i, ScreenItem& si);



/* SPELL SCREEN */
    template<typename T>
ScreenItem SpellScreen<T>::build_screen_item(TCODConsole* con, int i, T* element)
{
    ScreenItem result;

    char buffer[512];

    bool has_duration;
    bool is_chosen = this->is_chosen(element);
    bool is_active = this->is_active(element);
    has_duration = element->spell_effect->duration != -1;

    TCODColor foreground, background;
    std::vector<TCODColor> colors = this->get_colors(con, element);
    foreground = colors.at(0);
    background = colors.at(1);

    TCODConsole::setColorControl(TCOD_COLCTRL_1, foreground, background);
    TCODConsole::setColorControl(TCOD_COLCTRL_2, element->get_spell_color(), con->getDefaultBackground());
    TCODConsole::setColorControl(TCOD_COLCTRL_3, TCODColor::lightCyan, background);
    TCODConsole::setColorControl(TCOD_COLCTRL_4, TCODColor::white, background);

    //key, element, name
    std::string base_msg_str = "%c-%c%c%c %c%s%c : ";
    sprintf(buffer, base_msg_str.c_str(), this->key, TCOD_COLCTRL_2, 's',
            TCOD_COLCTRL_STOP, TCOD_COLCTRL_1, element->name.c_str(),
            TCOD_COLCTRL_STOP);

    //mana, range
    std::string msg_str = buffer;
    msg_str.append("%c%d mana%c, %c%drng%c");
    sprintf(buffer, msg_str.c_str(), TCOD_COLCTRL_3,
            element->mana_cost, TCOD_COLCTRL_STOP, TCOD_COLCTRL_4,
            element->max_range, TCOD_COLCTRL_STOP);
    msg_str = buffer;

    //duration
    if (has_duration)
    {
        msg_str.append(", %ddur");
        sprintf(buffer, msg_str.c_str(), element->spell_effect->duration);
        msg_str = buffer;
    };

    //aoe
    if (element->aoe > 0)
    {
        std::stringstream ss;
        ss << msg_str << ", " << element->aoe << "aoe";
        msg_str = ss.str();
    };

    if (is_chosen)
    {
        msg_str.append(" <-");
    }

    result.foreground = foreground;
    result.background = background;
    result.msg_str = msg_str;
    result.element = element;

    return result;
};
template ScreenItem SpellScreen<Spell>::build_screen_item(TCODConsole* con, int i, Spell* element);

    template<typename T>
void SpellScreen<T>::draw_screen_item(TCODConsole* con, int& i, ScreenItem& si)
{
    //print the item name and selection
    const char *msg_char = si.msg_str.c_str();
    si.min_y = i;
    con->printEx(this->offset, i, TCOD_bkgnd_flag_t::TCOD_BKGND_SET,
            TCOD_alignment_t::TCOD_LEFT, msg_char, this->key );
    i++;

    //print the item effects
    std::string msg = ((T*)si.element)->spell_effect->oneline_str();
    std::vector<TCOD_colctrl_t> colctrl_vec = ((T*)si.element)->spell_effect->oneline_str_colours();
    one_line_helper(con, offset, i, msg, colctrl_vec);
    si.max_y = i;

    i++;
    i++;
};
template void SpellScreen<Spell>::draw_screen_item(TCODConsole* con, int& i, ScreenItem& si);

/* SPELL SCREEN */
    template<typename T>
ScreenItem ClassScreen<T>::build_screen_item(TCODConsole* con, int i, T* element)
{
    ScreenItem result;

    char buffer[512];

    bool has_duration;
    bool is_chosen = this->is_chosen(element);
    bool is_active = this->is_active(element);

    TCODColor foreground, background;
    std::vector<TCODColor> colors = this->get_colors(con, element);
    foreground = colors.at(0);
    background = colors.at(1);

    TCODConsole::setColorControl(TCOD_COLCTRL_1, foreground, background);
    TCODConsole::setColorControl(TCOD_COLCTRL_2, TCODColor::green, con->getDefaultBackground());
    //TCODConsole::setColorControl(TCOD_COLCTRL_2, element->get_spell_color(), con->getDefaultBackground());
    TCODConsole::setColorControl(TCOD_COLCTRL_3, TCODColor::lightCyan, background);
    TCODConsole::setColorControl(TCOD_COLCTRL_4, TCODColor::white, background);

    //key, element, name
    std::string msg_str = "%c-%c%s%c";
    // std::string msg_str = "%c-%c%c%c %c%s%c : ";
    sprintf(buffer, msg_str.c_str(), this->key, TCOD_COLCTRL_1, 
            element->name.c_str(), TCOD_COLCTRL_STOP);
    msg_str = buffer;

    if (is_chosen)
    {
        msg_str.append(" <-");
    }

    result.foreground = foreground;
    result.background = background;
    result.msg_str = msg_str;
    result.element = element;

    return result;
};
template ScreenItem ClassScreen<IClass>::build_screen_item(TCODConsole* con, int i, IClass* element);

    template<typename T>
void ClassScreen<T>::draw_screen_item(TCODConsole* con, int& i, ScreenItem& si)
{
    //print the item name and selection
    const char *msg_char = si.msg_str.c_str();
    si.min_y = i;
    con->printEx(this->offset, i, TCOD_bkgnd_flag_t::TCOD_BKGND_SET,
            TCOD_alignment_t::TCOD_LEFT, msg_char);
    i++;

    //print the item effects
    //std::string msg = ((T*)si.element)->spell_effect->oneline_str();
    //std::vector<TCOD_colctrl_t> colctrl_vec = ((T*)si.element)->spell_effect->oneline_str_colours();
    //one_line_helper(con, offset, i, msg, colctrl_vec);
    si.max_y = i;

    i++;
    i++;
};
template void ClassScreen<IClass>::draw_screen_item(TCODConsole* con, int& i, ScreenItem& si);


