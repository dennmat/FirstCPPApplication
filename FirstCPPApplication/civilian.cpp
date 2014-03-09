#include "stdafx.h"
#include "civilian.h"

#include <vector>
#include <string>


Civilian::Civilian()
{
    this->master = NULL;
    this->chat_lines = new std::vector<std::string>();
    this->chat_lines->push_back("Nice day outside, isn't it?");
}

Civilian::~Civilian()
{
    delete this->chat_lines;
}

std::string Civilian::talk_general_topic()
{
    return this->chat_lines->back();
};
