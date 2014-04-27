#include "stdafx.h"
#include "civilian.h"


#include "libtcod_cpp_hpp\libtcod.hpp"


Civilian::Civilian()
{
    this->disposition = 50;
    this->master = NULL;
    this->chat_lines = new std::vector<std::string>();
    this->chat_lines->push_back("Nice day outside, isn't it? I wouldn't actually know.");
    this->chat_lines->push_back("I smell peanut butter.");
    this->chat_lines->push_back("You know what, I don't care for you at all.");
    this->chat_lines->push_back("If you had any sense at all, you'd be happy to let me touch you.");
    this->chat_lines->push_back("I'm pretty sure I'm trapped in here.");
}

Civilian::~Civilian()
{
    delete this->chat_lines;
}

std::string Civilian::talk_general_topic()
{
    int index = TCODRandom::getInstance()->getInt(0, this->chat_lines->size()-1);
    return this->chat_lines->at(index);
};
