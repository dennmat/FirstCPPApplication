#include "stdafx.h"
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
#include "map.h"
#include "tile.h"
#include "draw_utils.h"
#include "Representation.h"

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

unsigned long long int Ui::tick_mouse_moved = 0;
int Ui::mouse_move_threshold = 10;
unsigned long long int Ui::tick_key_pressed = 0;
unsigned long long int Ui::tick_key_released = 0;

bool Ui::is_targetting = false;

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
    draw_ui_sidebar();
    draw_ui_msg();
    draw_misc();
};

bool Ui::should_draw_mouse_helpbox()
{
    // return true;
    return Ui::tick_mouse_moved > Ui::tick_key_pressed;
};

void Ui::draw_mouse_helpbox()
{
    //get help text
    std::string help_text = "";
    Tile* mouse_tile = Game::get_mouse_tile();
    if (! mouse_tile->is_known())
        help_text = "Unknown tile";
    else if (mouse_tile->is_occupied())
        help_text = mouse_tile->occupant->name;
    else if (mouse_tile->inventory->get_count() > 0)
        help_text = mouse_tile->inventory->items->back()->name;
    else
        help_text = mouse_tile->tile->description;

    //draw the right sizes
    int left_pad=1, right_pad=2, top_pad=1, bot_pad=1;
    int help_text_width = help_text.size()+1;
    int help_text_height = 3;
    int adjusted_w = help_text_width+left_pad+right_pad;
    int adjusted_h = help_text_height+top_pad+bot_pad;
    TCODConsole help_con = TCODConsole(adjusted_w, adjusted_h);
    if (!mouse_tile->is_known())
        help_con.setDefaultForeground(TCODColor::darkestGrey);
    help_con.clear();

    if (!mouse_tile->is_known())
        draw_rect(&help_con, 0, 0, adjusted_w, adjusted_h, "?", "?", "?", "?", "?", "?", "?", "?");
    else
        draw_rect(&help_con, 0, 0, adjusted_w, adjusted_h);

    help_con.print(1+left_pad, 1+top_pad, help_text.c_str());

    TCODConsole::root->blit(&help_con, 0, 0, adjusted_w, adjusted_h, TCODConsole::root, Game::mouse_evt.cx+1, Game::mouse_evt.cy+1);
};

void Ui::draw_facing_angle(TCODConsole* ui_sidebar_con)
{
    //is moving in a direction
    bool ml = Game::player->is_moving_left;
    bool mr = Game::player->is_moving_right;
    bool mu = Game::player->is_moving_up;
    bool md = Game::player->is_moving_down;
    std::string sl = " ", sr = " ", st = " ", sb = " ", stl = " ", str = " ", sbl = " ", sbr = " ";

    //draw the line the right way
    if (ml && mu) { stl= "\\"; }
    else if (mr && mu) { str= "/"; }
    else if (mu) { st= "|"; }
    else if (mr && md) { sbr= "\\"; }
    else if (ml && md) { sbl= "/"; }
    else if (md) { sb= "|"; }
    else if (ml) { sl= "-"; }
    else if (mr) { sr= "-"; };

    ui_sidebar_con->print(1,  8, "%s%s%s", stl.c_str(), st.c_str(), str.c_str());
    ui_sidebar_con->print(1,  9, "%so%s", sl.c_str(), sr.c_str());
    ui_sidebar_con->print(1, 10, "%s%s%s", sbl.c_str(), sb.c_str(), sbr.c_str());

};

void Ui::draw_ui_sidebar()
{
    ui_sidebar_w = 20;
    ui_sidebar_h = Ui::game->screen_h-ui_msg_h;
    TCODConsole *ui_sidebar_con = new TCODConsole(ui_sidebar_w, ui_sidebar_h);

    if (Ui::should_draw_mouse_helpbox())
    {
        draw_mouse_helpbox();
    }
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
    ui_sidebar_con->print(0, 6, "%d %d", Ui::game->mouse_evt.cx+Game::camera_x, Ui::game->mouse_evt.cy+Game::camera_y);

    //facing direction
    Ui::draw_facing_angle(ui_sidebar_con);

    //draw player inventory info
    ui_sidebar_con->print(0, 12, "Items in inventory:");
    ui_sidebar_con->print(0, 13, "%d", Ui::game->player->inventory->get_count());

    int initial_y = 15;
    int y = initial_y;
    Ui::draw_xp(y, ui_sidebar_con, ui_sidebar_fore);
    y++;

    initial_y = y;

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



    //draw ui console to root
    TCODConsole::blit(ui_sidebar_con, 0, 0, ui_sidebar_w, ui_sidebar_h, TCODConsole::root, Ui::game->screen_w-ui_sidebar_w, 0 );
    delete ui_sidebar_con;
};

bool Ui::toggle_targetting()
{
    Ui::is_targetting = !Ui::is_targetting;
    return Ui::is_targetting;
};

void Ui::draw_misc()
{
    if (Ui::is_targetting)
    {
        Tile* mouse_tile = Game::get_mouse_tile();
	TCODColor line_color;
        if (mouse_tile->is_occupied() && mouse_tile->is_known())
        {
            TCODConsole::root->setChar(mouse_tile->tile_x, mouse_tile->tile_y, 'X');
            line_color = TCODColor::red;
        }
        else
        {
            line_color = TCODColor::grey;
        }

        // draw line from player to mouse
        TCODLine::init(Game::player->x, Game::player->y,
                Game::mouse_evt.cx, Game::mouse_evt.cy);
        int x = Game::player->x, y2 = Game::player->y ;
        do {
            TCODConsole::root->setCharBackground(x, y2, line_color);
        } while (!TCODLine::step(&x,&y2));

    }

}

void Ui::draw_xp(int& y, TCODConsole* ui_sidebar_con, TCODColor ui_sidebar_fore)
{
    // experience bar
    ui_sidebar_con->setDefaultForeground(ui_sidebar_fore);

    ui_sidebar_con->print(0, y, "XP LEVEL %d", Ui::game->player->level);
    y++;

    ui_sidebar_con->print(0, y, "XP TOTAL %d", Ui::game->player->xp);
    y++;

    std::string left_exp = ">";
    std::string right_exp = "";
    float left_percent = ((float)(Ui::game->player->xp_this_level)) / Ui::game->player->xp_required ;
    if (left_percent < 0) left_percent = 0.0f;
    if (left_percent > 1) left_percent = 1.0f;

    float right_raw = ((float)Ui::game->player->xp_required - (float)Ui::game->player->xp_this_level);
    float right_percent = right_raw / (float)Ui::game->player->xp_required;
    if (right_percent < 0) right_percent = 0.0f;
    if (right_percent > 1) right_percent = 1.0f;

    int left_count = floor(left_percent * (float)8);
    int right_count = floor(right_percent * (float)8);
    if (left_count + right_count < 8) right_count++;

    left_exp.append(left_count, '-');
    right_exp.append(right_count, '-');
    right_exp.append(1, '>');

    if (left_percent != 1.0f)
    {
        ui_sidebar_con->print(0, y, "%c%s%c%s", TCOD_COLCTRL_1, left_exp.c_str(), TCOD_COLCTRL_STOP, right_exp.c_str());
    }
    else //fill bar all the way when level max reached
    {
        ui_sidebar_con->print(0, y, "%c%s%s%c", TCOD_COLCTRL_1, left_exp.c_str(), right_exp.c_str(), TCOD_COLCTRL_STOP );
    }
    y++;

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
    Ui::msg_handler_main->draw(ui_msg_con);

    //draw ui console to root
    TCODConsole::blit(ui_msg_con, 0, 0, ui_msg_w, ui_msg_h, TCODConsole::root, 0, Ui::game->screen_h-ui_msg_h);
    delete ui_msg_con;
};

void Ui::draw_inventory_main()
{
    Ui::draw_inventory_ui();
    Ui::draw_inventory_msg();
};

void Ui::draw_screen_title(std::string title, TCODConsole* con)
{
    int inv_title_x = Ui::game->screen_w/2;
    TCOD_bkgnd_flag_t bkgnd_flag = con->getBackgroundFlag();
    con->printEx(inv_title_x, 2, bkgnd_flag, TCOD_alignment_t::TCOD_CENTER, title.c_str());

};

TCODConsole* Ui::create_screen()
{
    int con_w = Ui::game->screen_w;
    int con_h = Ui::game->screen_h - 10;
    TCODConsole *con = new TCODConsole(con_w, con_h);
    return con;
};

void Ui::format_attribute(Attribute* attr, char buffer[])
{
    std::string msg_template = "%i out of %i, regenerating %i every %i turns";
    sprintf(buffer, msg_template.c_str(), attr->current_val, attr->max_val, attr->regen_rate, attr->regen_interval);
};

void Ui::print_attribute(TCODConsole* con, Attribute* attr, char buffer[], int& i, std::string name)
{
    con->printEx(3, i, TCOD_bkgnd_flag_t::TCOD_BKGND_SET, TCOD_alignment_t::TCOD_LEFT, name.c_str());
    i++;
    Ui::format_attribute(attr, buffer);
    con->printEx(3, i, TCOD_bkgnd_flag_t::TCOD_BKGND_SET, TCOD_alignment_t::TCOD_LEFT, buffer);
    i++;
    i++;
};

void Ui::print_experience(TCODConsole* con, int& i)
{
    char buffer[100];
    Person* player = Game::player;
    con->printEx(3, i, TCOD_bkgnd_flag_t::TCOD_BKGND_SET, TCOD_alignment_t::TCOD_LEFT, "EXPERIENCE");
    i++;
    con->printEx(3, i, TCOD_bkgnd_flag_t::TCOD_BKGND_SET, TCOD_alignment_t::TCOD_LEFT, "Level %i", player->level);
    i++;

    std::string msg_template = "%i out of %i XP to next level, with %i total.";
    sprintf(buffer, msg_template.c_str(), player->xp_this_level, player->xp_required, player->xp);
    con->printEx(3, i, TCOD_bkgnd_flag_t::TCOD_BKGND_SET, TCOD_alignment_t::TCOD_LEFT, buffer);
};

void Ui::character_sheet_ui_loop(TCODConsole* con, int offset, int i, char key)
{
    TCODColor foreground, background;
    foreground = TCODColor::white;

    char buffer [100];

    std::string msg_str;
    AttributeContainer* player_attrs = Game::player->attrs;

    HealthAttribute* health = player_attrs->health;
    ManaAttribute* mana = player_attrs->mana;
    DamageAttribute* damage = player_attrs->damage;
    ArmorAttribute* armor = player_attrs->armor;

    Ui::print_attribute(con, health, buffer, i, "HEALTH");
    Ui::print_attribute(con, mana, buffer, i, "MANA");
    Ui::print_attribute(con, damage, buffer, i, "DAMAGE");
    Ui::print_attribute(con, armor, buffer, i, "ARMOR");

    print_experience(con, i);

};

void Ui::inventory_ui_loop(TCODConsole* con, int offset, int i, char key)
{
    TCODColor foreground, background;
    foreground = TCODColor::white;

    bool is_chosen, is_active;
    std::vector<Item*>* v  = Ui::game->player->inventory->items;
    for (std::vector<Item*>::iterator it = v->begin(); it != v->end(); ++it) 
    {
        std::string msg_str = "%c-%c%c%c %c%s%c";
        is_chosen = (*it) == Ui::chosen_item;
        is_active = Ui::item_active;

        TCODConsole::setColorControl(TCOD_COLCTRL_2, *(*it)->repr->fg_color, con->getDefaultBackground());
        if (Ui::game->player->equipment->is_item_equipped(*it))
        {
            background = TCODColor::darkestRed;
            if (is_chosen)
            {
                msg_str.append(" <-");
                // background = TCODColor::white;
                if (is_active)
                {
                    foreground = TCODColor::red+TCODColor::blue;
                }
                else 
                {
                    foreground = TCODColor::red+TCODColor::green;
                }
            }
            else
            {
                foreground = TCODColor::red;
            };
        }
        else
        {
            background = con->getDefaultBackground();
            if (is_chosen)
            {
                msg_str.append(" <-");
                if (is_active)
                {
                    foreground = TCODColor::red+TCODColor::yellow;
                }
                else
                {
                    // foreground = TCODColor::green;
                }
            }
            else
            {
                foreground = TCODColor::white;
            };
        };


        if (Game::mouse_evt.lbutton_pressed)
        {
            if (Game::mouse_evt.cy == i)
            {
                if ( (*it)!= Ui::chosen_item)
                {
                    Ui::chosen_item= (*it);
                    Ui::item_active = false;
                }
                else if ( (*it) == Ui::chosen_item)
                {
                    Ui::item_active = true;
                    background = TCODColor::green;
                };

            }
        }
        else if (Game::mouse_evt.rbutton_pressed)
        {
            Ui::chosen_item = NULL;
            Ui::item_active = false;
        };

        //print the item name and selection
        TCODConsole::setColorControl(TCOD_COLCTRL_1, foreground, background);
        const char *msg_char = msg_str.c_str();
        con->printEx(3, i, TCOD_bkgnd_flag_t::TCOD_BKGND_SET, TCOD_alignment_t::TCOD_LEFT, msg_char, key, TCOD_COLCTRL_2, (*it)->repr->repr, TCOD_COLCTRL_STOP, TCOD_COLCTRL_1, (*it)->name.c_str(), TCOD_COLCTRL_STOP);

        i++;

        //print the item effects
        std::string msg = (*it)->item_effect->oneline_str();
        std::vector<TCOD_colctrl_t> colctrl_vec = (*it)->item_effect->oneline_str_colours();
        one_line_helper(con, i, msg, colctrl_vec);
        i++;
        i++;

        key++;

    }
};

void Ui::draw_screen(std::string title, void (*loop_through_lines)(TCODConsole*, int, int, char))
{
    // clear the screen
    TCODConsole::root->clear();

    TCODConsole *con = Ui::create_screen();
    Ui::draw_screen_title(title, con);

    // draw the list of items on the player
    int offset = 5;
    int i = offset;
    char key = 'a';

    //draw mouse line
    int hline = 2;
    con->hline(0, Game::mouse_evt.cy, hline);
    con->putChar(hline, Game::mouse_evt.cy, '>');


    if (loop_through_lines != NULL)
    {
        loop_through_lines(con, offset, i, key);
    };

    TCODConsole::blit(con, 0, 0, ui_inv_w, ui_inv_h, TCODConsole::root, 0, 0);
    delete con;

};

void Ui::draw_inventory_ui()
{
    Ui::draw_screen("Inventory Screen", &Ui::inventory_ui_loop);
};

void Ui::draw_main_menu_ui()
{
    Ui::draw_screen("Main menu", NULL);
};

void Ui::draw_char_sheet_ui()
{
    Ui::draw_screen("Character Sheet", &Ui::character_sheet_ui_loop);
};

void Ui::draw_help_ui()
{
    Ui::draw_screen("Help Information", NULL);
};

void one_line_helper(TCODConsole* con, int i, std::string msg_str, std::vector<TCOD_colctrl_t> color_vector)
{
    //add a col stop to end
    if (color_vector.size() != 0)
        msg_str.append("%c");
    const char* msg = msg_str.c_str();

    int x = 4;
    if (color_vector.size() == 0)
        con->print(x, i, msg);
    else if (color_vector.size() == 1)
        con->print(x, i, msg, color_vector.at(0), TCOD_COLCTRL_STOP);
    else if (color_vector.size() == 2)
        con->print(x, i, msg, color_vector.at(0), color_vector.at(2-1), TCOD_COLCTRL_STOP);
    else if (color_vector.size() == 3)
        con->print(x, i, msg, color_vector.at(0), color_vector.at(2-1), color_vector.at(3-1), TCOD_COLCTRL_STOP);
    else if (color_vector.size() == 4)
        con->print(x, i, msg, color_vector.at(0), color_vector.at(2-1), color_vector.at(3-1), color_vector.at(4-1), TCOD_COLCTRL_STOP);
    else if (color_vector.size() == 5)
        con->print(x, i, msg, color_vector.at(0), color_vector.at(2-1), color_vector.at(3-1), color_vector.at(4-1), color_vector.at(5-1), TCOD_COLCTRL_STOP);
    else if (color_vector.size() == 6)
        con->print(x, i, msg, color_vector.at(0), color_vector.at(2-1), color_vector.at(3-1), color_vector.at(4-1), color_vector.at(5-1), color_vector.at(6-1), TCOD_COLCTRL_STOP);
    else if (color_vector.size() == 7)
        con->print(x, i, msg, color_vector.at(0), color_vector.at(2-1), color_vector.at(3-1), color_vector.at(4-1), color_vector.at(5-1), color_vector.at(6-1), color_vector.at(7-1), TCOD_COLCTRL_STOP);
    else if (color_vector.size() == 8)
        con->print(x, i, msg, color_vector.at(0), color_vector.at(2-1), color_vector.at(3-1), color_vector.at(4-1), color_vector.at(5-1), color_vector.at(6-1), color_vector.at(7-1), color_vector.at(8-1), TCOD_COLCTRL_STOP);
    else if (color_vector.size() == 9)
        con->print(x, i, msg, color_vector.at(0), color_vector.at(2-1), color_vector.at(3-1), color_vector.at(4-1), color_vector.at(5-1), color_vector.at(6-1), color_vector.at(7-1), color_vector.at(8-1), color_vector.at(9-1), TCOD_COLCTRL_STOP);
    else if (color_vector.size() == 10)
        con->print(x, i, msg, color_vector.at(0), color_vector.at(2-1), color_vector.at(3-1), color_vector.at(4-1), color_vector.at(5-1), color_vector.at(6-1), color_vector.at(7-1), color_vector.at(8-1), color_vector.at(9-1), color_vector.at(10-1), TCOD_COLCTRL_STOP);
    else if (color_vector.size() == 11)
        con->print(x, i, msg, color_vector.at(0), color_vector.at(2-1), color_vector.at(3-1), color_vector.at(4-1), color_vector.at(5-1), color_vector.at(6-1), color_vector.at(7-1), color_vector.at(8-1), color_vector.at(9-1), color_vector.at(10-1), color_vector.at(11-1), TCOD_COLCTRL_STOP);
    else if (color_vector.size() == 12)
        con->print(x, i, msg, color_vector.at(0), color_vector.at(2-1), color_vector.at(3-1), color_vector.at(4-1), color_vector.at(5-1), color_vector.at(6-1), color_vector.at(7-1), color_vector.at(8-1), color_vector.at(9-1), color_vector.at(10-1), color_vector.at(11-1), color_vector.at(12-1), TCOD_COLCTRL_STOP);
    else if (color_vector.size() == 13)
        con->print(x, i, msg, color_vector.at(0), color_vector.at(2-1), color_vector.at(3-1), color_vector.at(4-1), color_vector.at(5-1), color_vector.at(6-1), color_vector.at(7-1), color_vector.at(8-1), color_vector.at(9-1), color_vector.at(10-1), color_vector.at(11-1), color_vector.at(12-1), color_vector.at(13-1), TCOD_COLCTRL_STOP);
    else if (color_vector.size() == 14)
        con->print(x, i, msg, color_vector.at(0), color_vector.at(2-1), color_vector.at(3-1), color_vector.at(4-1), color_vector.at(5-1), color_vector.at(6-1), color_vector.at(7-1), color_vector.at(8-1), color_vector.at(9-1), color_vector.at(10-1), color_vector.at(11-1), color_vector.at(12-1), color_vector.at(13-1), color_vector.at(14-1), TCOD_COLCTRL_STOP);
    else if (color_vector.size() == 15)
        con->print(x, i, msg, color_vector.at(0), color_vector.at(2-1), color_vector.at(3-1), color_vector.at(4-1), color_vector.at(5-1), color_vector.at(6-1), color_vector.at(7-1), color_vector.at(8-1), color_vector.at(9-1), color_vector.at(10-1), color_vector.at(11-1), color_vector.at(12-1), color_vector.at(13-1), color_vector.at(14-1), color_vector.at(15-1), TCOD_COLCTRL_STOP);
    else if (color_vector.size() == 16)
        con->print(x, i, msg, color_vector.at(0), color_vector.at(2-1), color_vector.at(3-1), color_vector.at(4-1), color_vector.at(5-1), color_vector.at(6-1), color_vector.at(7-1), color_vector.at(8-1), color_vector.at(9-1), color_vector.at(10-1), color_vector.at(11-1), color_vector.at(12-1), color_vector.at(13-1), color_vector.at(14-1), color_vector.at(15-1), color_vector.at(16-1), TCOD_COLCTRL_STOP);
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
    int y = 0;
    ui_inv_msg_con->setDefaultForeground(TCODColor::lightGrey+TCODColor::yellow);
    ui_inv_msg_con->print(0, y++, "Press the desired item's letter once to select it, and once more to confirm");
    ui_inv_msg_con->print(0, y++, "You can then press E to equip, Y to unequip, U to use, D to drop, Q to deselect item.");
    y++;
    ui_inv_msg_con->print(0, y++, "Use corpses and potions, equip swords and helms.");
    ui_inv_msg_con->print(0, y++, "You need a free slot to equip anything, naturally.");

    ui_inv_msg_con->setDefaultForeground(TCODColor::white);
    y++;
    ui_inv_msg_con->print(0, y++, "is item chosen? %i", Ui::item_is_chosen());
    ui_inv_msg_con->print(0, y++, "is item confirmed? %i", Ui::item_active);

    //draw ui console to root
    TCODConsole::blit(ui_inv_msg_con, 0, 0, ui_inv_msg_w, ui_inv_msg_h, TCODConsole::root, 0, Ui::game->screen_h-ui_inv_msg_h);
    delete ui_inv_msg_con;
};

bool Ui::item_is_chosen()
{
    return Ui::chosen_item != NULL;
};
