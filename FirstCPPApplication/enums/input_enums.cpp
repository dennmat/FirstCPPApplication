#include "stdafx.h"
#include "input_enums.h"

const char* basic_cmds_char[] = {
    "Pickup", "Drop",
    "OpenInventory",
    "Look", "ActivateDoor",
    "DownStairs", "UpStairs",
    "OpenMagic", "ConfirmCast",
    "OpenCharacterSheet",
    "OpenHelp", "OpenClassSelect",
    "CenterScreenToMouse",
    "NO_MATCHING_BASIC_CMD"
};

const char* inventory_items_active_char[] = {
    "ExamineItem",
    "EquipItem", "UnequipItem",
    "DropItem", "EscapeMenuItem",
    "UseItem",
    "NO_MATCHING_ITEMS_ACTIVE"
};

const char* generic_menu_active_char[] = {
    "EscapeGenericMenu",
    "NO_MATCHING_GENERIC_MENU_ACTIVE"
};

const char* spells_active_char[] = {
    "ExamineSpell",
    "EquipSpell", "UnequipSpell",
    "DropSpell", "EscapeMenuSpell",
    "CastSpell",
    "NO_MATCHING_SPELLS_ACTIVE"
};

const char* classes_active_char[] = {
    "ExamineClass",
    "EquipClass", "UnequipClass",
    "DropClass", "EscapeMenuClass",
    "CastClass",
    "NO_MATCHING_CLASSES_ACTIVE"
};
