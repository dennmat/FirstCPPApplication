#pragma once
#ifndef SCREEN_H
#define SCREEN_H


template< typename T>
class Screen 
{
    public: 
        std::string title;

        int offset;
        char key;
        int hline;

        std::vector<T*>* elements;


        Screen();

        TCODConsole* create_screen();

        void draw_screen_title(TCODConsole* con);
        void draw_mouse_horiz_line(TCODConsole* con);
        virtual void loop(TCODConsole* con, int i) = 0;
        virtual void build_screen_items(TCODConsole* con, int i) = 0;

        virtual void draw();

};

template<typename T>
class InventoryScreen : public Screen<T>
{
    public:

        InventoryScreen<T>() : Screen() { this->title="Inventory Screen"; };
	InventoryScreen::~InventoryScreen(){};
        void build_screen_items(TCODConsole* con, int i);
        void loop(TCODConsole* con, int i);

        // void draw();
};

class ScreenItem
{

};

#endif
