#pragma once

#ifndef FirstCPPApplication_h
#define FirstCPPApplication_h

#include <sstream>
#include <string>

#include "person.h"


class Person;

void WelcomeMessage();
void clearScreen();
void process_movement(std::string request, Person *player);
void process_request(std::string request, Person *player);

#endif
