#pragma once
#ifndef UI_H
#define UI_H

#include "libtcod_cpp_hpp\libtcod.hpp"

class Game;

class Ui
{
    public:
        Game* game;

        int ui_msg_w;
        int ui_msg_h;
        int ui_sidebar_w;
        int ui_sidebar_h;

        void update_ui();

        void draw_ui();
        void draw_ui_msg();
        void draw_ui_sidebar();

};

#endif
