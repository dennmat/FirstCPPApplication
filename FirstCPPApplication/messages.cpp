#include "stdafx.h"
#include <string>
#include "messages.h"
#include <vector>

#include <libtcod.hpp>

MessageHandler::MessageHandler()
{
    this->msg_list = std::vector<Message*>();
};

void MessageHandler::draw(TCODConsole* console)
{
    //go through the latest messages and draw them until there's no more room
    std::vector<Message*>::reverse_iterator it = this->msg_list.rbegin();
    int x = 0;
    int y = 0;
    for (it; it != this->msg_list.rend(); ++it)
    {
        console->print(x, y, (*it)->content.c_str(), (*it)->vlist);
        y++;
    };

};

void Message::Init()
{
    this->content = "Unspecified content";
    this->vlist = NULL;
};

Message::Message()
{
    this->Init();
};

Message::Message(std::string content, ...) 
{
    this->Init();

    this->content = content;
    this->vlist = vlist;
};
