#ifndef INPUT_H
#define INPUT_H

#pragma once
#include "stdafx.h"
#include <map>
#include <iostream>

#include "libtcod.hpp"


class Person;
class Map;
class Game;

bool process_mouse_event(Game* the_game,TCOD_mouse_t request, Person *player);
bool process_key_event(Game* the_game,TCOD_key_t request, Person *player);

extern enum directions_t;
directions_t direction_pressed(TCOD_key_t key);
bool is_request_move_cmd(TCOD_key_t request);
bool process_movement(Game* the_game,TCOD_key_t request, Person *player);

extern enum basic_cmds_t;
basic_cmds_t  basic_cmd_pressed(TCOD_key_t key);
bool is_request_basic_cmd(TCOD_key_t request);
bool process_basic_cmd(Game* the_game, TCOD_key_t request, Person *player);


#endif
