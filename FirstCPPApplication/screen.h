#pragma once
#ifndef SCREEN_H
#define SCREEN_H


class Screen 
{
    public: 
        std::string title;

        int offset;
        char key;
        int hline;

        Screen();

        void drawn_screen_title(TCODConsole* con);
        void draw_mouse_horiz_line(TCODConsole* con);
        virtual void loop(TCODConsole* con, int i) = 0;
        virtual void draw() = 0;
};

class InventoryScreen : private Screen
{
    public:
        void loop(TCODConsole* con, int i);

        InventoryScreen() : Screen() {};
        void draw();
};

class ScreenItem
{

};

#endif
