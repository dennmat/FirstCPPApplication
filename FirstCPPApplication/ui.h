#pragma once
#ifndef UI_H
#define UI_H

#include "libtcod_cpp_hpp\libtcod.hpp"

class Item;
class Game;

class Ui
{
    private:
        unsigned long long int tick_threshold;
        unsigned long long int tick_turn_changed;
        unsigned long long int last_turn_noted;
        unsigned long long int turn_checking_against;
        unsigned long long int tick_checking_against;
    public:
        Game* game;

        Ui();

        int ui_inv_w;
        int ui_inv_h;
        int ui_inv_msg_w;
        int ui_inv_msg_h;

        int ui_msg_w;
        int ui_msg_h;
        int ui_sidebar_w;
        int ui_sidebar_h;

        Item* chosen_item;
        bool item_active;

        void update_ui();
        void update_inventory_ui();

        void draw_ui();
        void draw_ui_msg();
        void draw_ui_sidebar();

        void draw_inventory_ui();
        void draw_inventory_main();
        void draw_inventory_msg();

        bool item_is_chosen();

};

#endif
