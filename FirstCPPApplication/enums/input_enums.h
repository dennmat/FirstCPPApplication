#pragma once
#ifndef INPUT_ENUMS_H
#define INPUT_ENUMS_H

// enum directions_t;
// enum inventory_items_active_t;
// enum basic_cmds_t;
// enum classes_active_t;
// enum spells_active_t;
// enum generic_menu_active_t;
// extern const char* directions_char[];
// extern const char* inventory_items_active_char[];
// extern const char* basic_cmds_char[];
// extern const char* classes_active_char[];
// extern const char* spells_active_char[];
// extern const char* generic_menu_active_char[];

enum directions_t {
    NW=0, N, NE,
    W,    X,  E,
    SW,   S, SE,
    NO_MATCHING_DIRECTION
};

enum basic_cmds_t {
    Pickup = 0, Drop,
    OpenInventory,
    Look, ActivateDoor,
    DownStairs, UpStairs,
    OpenMagic, ConfirmCast,
    OpenCharacterSheet,
    OpenHelp, OpenClassSelect,
    CenterScreenToMouse,
    NO_MATCHING_BASIC_CMD
};


enum inventory_items_active_t {
    ExamineItem,
    EquipItem, UnequipItem,
    DropItem, EscapeMenuItem,
    UseItem,
    NO_MATCHING_ITEMS_ACTIVE
};


enum generic_menu_active_t {
    EscapeGenericMenu,
    NO_MATCHING_GENERIC_MENU_ACTIVE
};


enum spells_active_t {
    ExamineSpell,
    EquipSpell, UnequipSpell,
    DropSpell, EscapeMenuSpell,
    CastSpell,
    NO_MATCHING_SPELLS_ACTIVE
};

enum classes_active_t {
    ExamineClass,
    EquipClass, UnequipClass,
    DropClass, EscapeMenuClass,
    CastClass,
    NO_MATCHING_CLASSES_ACTIVE
};
#endif
