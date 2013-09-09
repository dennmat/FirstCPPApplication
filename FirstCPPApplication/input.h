#ifndef INPUT_H
#define INPUT_H

#include "stdafx.h"

#include "Person.h"


#include "libtcod.hpp"

class Person;
class Map;
class Game;


void process_movement(Game* the_game,TCOD_key_t request, Person *player);
bool process_mouse_event(Game* the_game,TCOD_mouse_t request, Person *player);
bool process_key_event(Game* the_game,TCOD_key_t request, Person *player);

#endif
