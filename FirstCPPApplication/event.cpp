#include "stdafx.h"
#include "event.h"
#include "actors\actor.h"
#include "item.h"
#include "tile.h"

Event::Event()
{

};

void Event::initialize()
{
    this->message = "UNSET";
    this->actors = NULL;
    this->items = NULL;
    this->tiles = NULL;
};

Event::Event(std::string message, std::vector<Actor*>* actors, std::vector<Item*>* items, std::vector<Tile*>* tiles)
{
    this->message = message;
    this->actors = actors;
    this->items = items;
    this->tiles = tiles;
};

void Event::activate()
{

};
