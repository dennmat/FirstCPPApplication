#pragma once
#ifndef EVENT_H
#define EVENT_H

class Actor;
class Item;
class Tile;

class Event
{
    public:
        bool is_active;

        std::string message; //once the event happens, thisll get posted
        std::vector<Actor*>* actors;
        std::vector<Item*>* items;
        std::vector<Tile*>* tiles;

        Event();

        void activate();
};

#endif
