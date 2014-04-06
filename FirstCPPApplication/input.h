#pragma once
#ifndef INPUT_H
#define INPUT_H

#include <map>
#include <iostream>
#include <unordered_map>

// #include "libtcod.hpp"
// #include "libtcod_cpp_hpp\libtcod.hpp"
// #include "libtcod_cpp_hpp\libtcod.hpp"
#include "libtcod_cpp_hpp\mouse_types.h"
#include "libtcod_cpp_hpp\console_types.h"


class Person;
class Map;
class Game;

bool process_debug_event(TCOD_key_t request);
bool process_mouse_event(TCOD_mouse_t request);
void process_mouse_inv_event();
bool process_key_event(TCOD_key_t request);

extern enum directions_t;
directions_t direction_pressed(TCOD_key_t key);
bool is_request_move_cmd(TCOD_key_t request);
bool process_movement(TCOD_key_t request);

extern enum inventory_items_active_t;
inventory_items_active_t inventory_items_active_pressed(TCOD_key_t key);
bool is_request_inventory_item_active_cmd(TCOD_key_t request);
bool process_inventory_item_active(TCOD_key_t request);

extern enum basic_cmds_t;
basic_cmds_t  basic_cmd_pressed(TCOD_key_t key);
bool is_request_basic_cmd(TCOD_key_t request);
bool process_basic_cmd(TCOD_key_t request);

typedef std::unordered_map<char, int> generic_keypair_t;
typedef std::pair<char, int> generic_keypair;
generic_keypair_t build_keypairs(int limit);

template<class T>
void select_generic(TCOD_key_t request, std::vector<T*>* generic_vector, bool (*active_func)(TCOD_key_t), bool (*process_func)(TCOD_key_t));

template<typename T1, typename T2, typename T3>
T1 find_key(TCOD_key_t key, T2 char_active_map, T3 spec_active_map, T1 error_choice);

#endif
