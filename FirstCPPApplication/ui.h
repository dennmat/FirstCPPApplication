#pragma once
#pragma comment (lib, "FirstCPPApplication.lib")
#ifndef UI_H
#define UI_H

#include "libtcod_cpp_hpp\libtcod.hpp"
#include "messages.h"

class Item;
class Game;
class TCODColor;
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
        static void draw_facing_angle(TCODConsole* ui_sidebar_con);
        static void draw_xp();
        static void draw_xp(int& y, TCODConsole* ui_sidebar_con, TCODColor ui_sidebar_fore);

        static void draw_inventory_ui();
        static void draw_inventory_main();
        static void draw_inventory_msg();

        static bool item_is_chosen();

};

#endif
