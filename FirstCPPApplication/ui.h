#pragma once
#pragma comment (lib, "FirstCPPApplication.lib")
#ifndef UI_H
#define UI_H

#include "libtcod_cpp_hpp\libtcod.hpp"
#include "messages.h"

class Item;
class Game;
class TCODColor;
class Attribute;
class TCODConsole;

void one_line_helper(TCODConsole* ui_inv_con, int i, std::string msg_str, std::vector<TCOD_colctrl_t> color_vector);

class Ui
{
    // private:
    public:
        static unsigned long long int tick_threshold;
        static unsigned long long int tick_turn_changed;
        static unsigned long long int last_turn_noted;
        static unsigned long long int turn_checking_against;
        static unsigned long long int tick_checking_against;

        static unsigned long long int tick_mouse_moved;
        static int mouse_move_threshold;
        static unsigned long long int tick_key_pressed;
        static unsigned long long int tick_key_released;

        static bool is_targetting;

        static Game* game;
        static MessageHandler* msg_handler_main;

        static int ui_inv_w;
        static int ui_inv_h;
        static int ui_inv_msg_w;
        static int ui_inv_msg_h;

        static int ui_msg_w;
        static int ui_msg_h;
        static int ui_sidebar_w;
        static int ui_sidebar_h;

        static Item* chosen_item;
        static bool item_active;

        static void update_ui();
        static void update_inventory_ui();

        static void draw_ui();
        static void draw_ui_msg();
        static void draw_ui_sidebar();
        static void draw_misc();
        static void draw_facing_angle(TCODConsole* ui_sidebar_con);
        static bool should_draw_mouse_helpbox();
        static void draw_mouse_helpbox();
        static void draw_xp();
        static void draw_xp(int& y, TCODConsole* ui_sidebar_con, TCODColor ui_sidebar_fore);

        static TCODConsole* create_screen();
        static void draw_screen_title(std::string title, TCODConsole* con);
        static void draw_screen(std::string title, void (*loop_through_lines)(TCODConsole*, int, int, char));

        static void draw_main_menu_ui();

        static void Ui::format_attribute(Attribute* attr, char buffer[]);
        static void Ui::print_attribute(TCODConsole* con, Attribute* attr, char buffer[], int& i, std::string name);
        static void Ui::print_experience(TCODConsole* con, int& i);
        static void draw_char_sheet_ui();
        static void character_sheet_ui_loop(TCODConsole* con, int offset, int i, char key);

        static void Ui::draw_help_ui();

        static void draw_inventory_main();
        static void draw_inventory_ui();
        static void draw_inventory_msg();
        static void inventory_ui_loop(TCODConsole* con, int offset, int i, char key);

        static bool item_is_chosen();
        static bool toggle_targetting();

};

#endif
