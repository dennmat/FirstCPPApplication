#pragma once
#ifndef UI_H
#define UI_H

#include "libtcod_cpp_hpp\libtcod.hpp"

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

        int ui_msg_w;
        int ui_msg_h;
        int ui_sidebar_w;
        int ui_sidebar_h;

        void update_ui();
        void update_inventory_ui();

        void draw_ui();
        void draw_ui_msg();
        void draw_ui_sidebar();

        void Ui::draw_inventory_ui();
};

#endif
