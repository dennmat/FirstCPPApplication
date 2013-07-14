#pragma once

#ifndef FirstCPPApplication_h
#define FirstCPPApplication_h

#include <sstream>
#include <string>

#include "person.h"

#include "libtcod.hpp"


class Person;

void WelcomeMessage();
void clearScreen();
void process_movement(TCOD_key_t request, Person *player);
void process_request(TCOD_key_t request, Person *player);

#endif
