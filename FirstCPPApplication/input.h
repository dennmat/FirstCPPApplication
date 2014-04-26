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

#include "enums\input_enums.h"


class Person;
class Map;
class Game;

typedef std::unordered_map<char, int> generic_keypair_t;
typedef std::pair<char, int> generic_keypair;
class Input 
{
    public:
        static std::vector<std::string> directions_char;
        static std::vector<std::string> inventory_items_active_char;
        static std::vector<std::string> basic_cmds_char;
        static std::vector<std::string> classes_active_char;
        static std::vector<std::string> spells_active_char;
        static std::vector<std::string> generic_menu_active_char;

        static bool process_debug_event(TCOD_key_t request);
        static bool process_mouse_event(TCOD_mouse_t request);
        static void process_mouse_inv_event();
        static bool process_key_event(TCOD_key_t request);

        static directions_t direction_pressed(TCOD_key_t key);
        static bool is_key_move_command(TCOD_key_t request);
        static bool process_movement_keys(TCOD_key_t request);

        static std::map<int, directions_t> spec_movemap; //Keypad, punctuation
        static std::map<char, directions_t> char_movemap; //regular letters
        static std::map<int, directions_t> build_spec_movemap(); //Keypad, punctuation
        static std::map<char, directions_t> build_char_movemap(); //regular letters

        static inventory_items_active_t inventory_items_active_pressed(TCOD_key_t key);
        static bool is_key_inventory_command(TCOD_key_t request);
        static bool process_inventory_keys(TCOD_key_t request);
        static std::map<int, inventory_items_active_t> spec_invitemactivemap; //Keypad, punctuation
        static std::map<char, inventory_items_active_t> char_invitemactivemap; //regular letters
        static std::map<int, inventory_items_active_t> build_spec_invitemactivemap(); //Keypad, punctuation
        static std::map<char, inventory_items_active_t> build_char_invitemactivemap(); //regular letters

        static generic_menu_active_t generic_menu_active_pressed(TCOD_key_t key);
        static bool process_generic_menu_keys(TCOD_key_t request);
        static bool is_key_generic_menu_command(TCOD_key_t request);
        static std::map<int, generic_menu_active_t> spec_genmenactivemap; //Keypad, punctuation
        static std::map<char, generic_menu_active_t> char_genmenactivemap; //regular letters
        static std::map<int, generic_menu_active_t> build_spec_genmenactivemap(); //Keypad, punctuation
        static std::map<char, generic_menu_active_t> build_char_genmenactivemap(); //regular letters

        static basic_cmds_t  basic_cmd_pressed(TCOD_key_t key);
        static bool is_key_basic_command(TCOD_key_t request);
        static bool process_basic_keys(TCOD_key_t request);
        static std::map<int, basic_cmds_t> spec_active_map; //Keypad, punctuation
        static std::map<char, basic_cmds_t> char_active_map; //regular letters
        static std::map<int, basic_cmds_t> build_spec_active_map(); //Keypad, punctuation
        static std::map<char, basic_cmds_t> build_char_active_map(); //regular letters

        static spells_active_t  spells_active_pressed(TCOD_key_t key);
        static bool is_key_spell_command(TCOD_key_t request);
        static bool process_spells_keys(TCOD_key_t request);
        static std::map<int, spells_active_t> spec_spellactivemap; //Keypad, punctuation
        static std::map<char, spells_active_t> char_spellactivemap; //regular letters
        static std::map<int, spells_active_t> build_spec_spellactivemap(); //Keypad, punctuation
        static std::map<char, spells_active_t> build_char_spellactivemap(); //regular letters

        static classes_active_t  classes_active_pressed(TCOD_key_t key);
        static bool is_key_class_command(TCOD_key_t request);
        static bool process_classes_keys(TCOD_key_t request);
        static std::map<int, classes_active_t> spec_classactivemap; //Keypad, punctuation
        static std::map<char, classes_active_t> char_classactivemap; //regular letters
        static std::map<int, classes_active_t> build_spec_classactivemap(); //Keypad, punctuation
        static std::map<char, classes_active_t> build_char_classactivemap(); //regular letters

        static generic_keypair_t build_keypairs(int limit, int offset);

        template<class T>
            static void select_generic(TCOD_key_t request, std::vector<T*>* generic_vector, bool (*active_func)(TCOD_key_t), bool (*process_func)(TCOD_key_t));

        template<typename T1, typename T2, typename T3>
            static T1 find_key(TCOD_key_t key, T2 char_active_map, T3 spec_active_map, T1 error_choice);

        static std::map<int, std::string> get_tcodkey_to_string_map();
};

#endif
