#ifndef INPUT_H
#define INPUT_H

#pragma once
#include <map>
#include <iostream>

// #include "libtcod.hpp"
// #include "libtcod_cpp_hpp\libtcod.hpp"
// #include "libtcod_cpp_hpp\libtcod.hpp"
#include "libtcod_cpp_hpp\mouse_types.h"
#include "libtcod_cpp_hpp\console_types.h"


class Person;
class Map;
class Game;

bool process_debug_event(TCOD_key_t request, Person *player);
bool process_mouse_event(TCOD_mouse_t request, Person *player);
void process_mouse_inv_event();
bool process_key_event(TCOD_key_t request, Person *player);

extern enum directions_t;
directions_t direction_pressed(TCOD_key_t key);
bool is_request_move_cmd(TCOD_key_t request);
bool process_movement(TCOD_key_t request, Person *player);

extern enum inventory_items_active_t;
inventory_items_active_t inventory_items_active_pressed(TCOD_key_t key);
bool is_request_inventory_item_active_cmd(TCOD_key_t request);
bool process_inventory_item_active(TCOD_key_t request, Person *player);

extern enum basic_cmds_t;
basic_cmds_t  basic_cmd_pressed(TCOD_key_t key);
bool is_request_basic_cmd(TCOD_key_t request);
bool process_basic_cmd(TCOD_key_t request, Person *player);

int get_euclidean_distance(int x1, int y1, int x2, int y2);

#endif
