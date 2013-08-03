#pragma once

#ifndef FirstCPPApplication_h
#define FirstCPPApplication_h

#include <sstream>
#include <string>

#include "person.h"

//#include "libtcod.hpp"
#include "../FirstCPPApplication/libtcod_cpp_hpp/libtcod.hpp"


class Person;

void WelcomeMessage();
void clearScreen();
void process_movement(TCOD_key_t request, Person *player);
bool process_event(TCOD_key_t request, Person *player);

#endif
