#include "stdafx.h"
#include <iostream>
#include <string>
#include "messages.h"
#include <vector>
#include <stdarg.h>

#include <libtcod.hpp>

MessageHandler::MessageHandler()
{
    this->msg_list = std::vector<Message*>();
    Message* msg = new Message;
    msg->content = "Welcome to BiochRL++";
    //msg->vlist = " ";
    msg->count = 0;
    this->msg_list.push_back(msg);
};

void MessageHandler::New(Message* message)
{


    Message* last_msg = this->msg_list.back();
    //compare vlists
    bool vlists_equal = false;
    // va_list new_list = message->vlist;
    // va_list old_list = last_msg->vlist;
    // if (new_list == old_list || (new_list == NULL && old_list == NULL))
    // {
    //     vlists_equal = true;
    // };

    // char* x = new char[999*32];
    // char* y = new char[999*32];
    // sprintf(x, message->content.c_str(), message->vlist);
    // sprintf(y, last_msg->content.c_str(), last_msg->vlist);
    // if (x == y)
    // {
    //     vlists_equal = true;
    // };

    if (message->content == last_msg->content)
	{
        vlists_equal = true;
	}
    else
	{
        vlists_equal = false;
	}






    if (vlists_equal && this->msg_list.size() != 0)
        //if (last_msg->content.c_str() == message->content.c_str() && last_msg->vlist == message->vlist && this->msg_list.size() != 0)
    {
        last_msg->count += 1;
        std::cout << "msg already existed, incrementing count" << std::endl;
    }
    else
    {
        this->msg_list.push_back(message);
    }
}

void MessageHandler::draw(TCODConsole* console)
{
    //go through the latest messages and draw them until there's no more room
    std::vector<Message*>::reverse_iterator it = this->msg_list.rbegin();
    int x = 0;
    int y = 0;
    for (it; it != this->msg_list.rend(); ++it)
    {
        // va_list ap;
        // va_start(ap, (*it)->content);
        std::cout << "drawing message" << std::endl;
        if ((*it)->count != 0)
        {
            console->print(x, y, ((*it)->content+" (%d)").c_str(), (*it)->count);
        }
        else 
        {
            console->print(x, y, (*it)->content.c_str());
        };
        y++;
        // va_end(ap);
    };

};

void Message::Init()
{
    this->content = "Unspecified %s";
    // this->vlist = "content";
    this->count = 0;
};

Message::Message()
{
    this->Init();
};

Message::Message(MessageHandler* handler, std::string content, ...) 
{

    this->Init();

    va_list ap;
    va_start(ap, content);

    char* x = new char[999*32];
    vsprintf(x, content.c_str(), ap);
    this->content = std::string(x);
    // this->vlist = vlist;
    va_end(ap);

    handler->New(this);
};

//std::string Message::Prerender()
//{
//    return "";
//}
