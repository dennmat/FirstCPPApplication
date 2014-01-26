#include "stdafx.h"
#include <iostream>
#include <string>
#include "messages.h"
#include <vector>

#include <libtcod.hpp>

MessageHandler::MessageHandler()
{
    this->msg_list = std::vector<Message*>();
    Message* msg = new Message;
    msg->content = "Welcome to BiochRL++";
    msg->vlist = "";
    this->msg_list.push_back(msg);
};

void MessageHandler::New(Message* message)
{

 //   if (this->msg_list.size() == 0)
	//{
	//return;
	//};
    Message* last_msg = this->msg_list.back();
    if (last_msg->content.c_str() == message->content.c_str() && last_msg->vlist == message->vlist && this->msg_list.size() != 0)
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
        std::cout << "drawing message" << std::endl;
        console->print(x, y, ((*it)->content+" (%d)").c_str(), (*it)->vlist, (*it)->count);
        y++;
    };

};

void Message::Init()
{
    this->content = "Unspecified content";
    this->vlist = NULL;
    this->count = 0;
};

Message::Message()
{
    this->Init();
};

Message::Message(MessageHandler* handler, std::string content, ...) 
{
    handler->New(this);

    this->Init();

    this->content = content;
    this->vlist = vlist;
};

//std::string Message::Prerender()
//{
//    return "";
//}
