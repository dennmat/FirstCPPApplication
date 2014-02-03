#include "stdafx.h"
#include <iostream>
#include <string>
#include "messages.h"
#include <vector>
#include <stdarg.h>

#include <libtcod.hpp>
#include "game.h"

MessageHandler::MessageHandler()
{
    this->msg_list = std::vector<Message*>();
    Message* msg = new Message;
    msg->content = "Welcome to BiochRL++";
    //msg->vlist = " ";
    msg->count = 0;
    this->msg_list.push_back(msg);
};

void MessageHandler::new_msg(Message* message)
{


    Message* last_msg = this->msg_list.back();
    //compare messages to see if the count should increment
    bool same_msg = false;
    if (message->content == last_msg->content)
	{
        same_msg = true;
	}

    if (same_msg && this->msg_list.size() != 0)
        //if (last_msg->content.c_str() == message->content.c_str() && last_msg->vlist == message->vlist && this->msg_list.size() != 0)
    {
        last_msg->count += 1;
        //std::cout << "msg already existed, incrementing count" << std::endl;
    }
    else
    {
        this->msg_list.push_back(message);
    }
}

void MessageHandler::draw_raw(TCODConsole* console)
{
    //go through the latest messages and draw them until there's no more room
    std::vector<Message*>::reverse_iterator it = this->msg_list.rbegin();
    int x = 0;
    int y = 0;
    TCODColor default_color = console->getDefaultForeground();
    for (it; it != this->msg_list.rend(); ++it)
    {
        float coef = ((float)y)/10.0f;
        TCODColor new_color = TCODColor::lerp(default_color, TCODColor::darkGrey, coef);
        console->setDefaultForeground(new_color);
        if ((*it)->count > 0)
        {
            console->print(x, y, ((*it)->content+" (x%d)").c_str(), (*it)->count);
        }
        else 
        {
            console->print(x, y, (*it)->content.c_str());
        };
        y++;

        if (y > 10) break; //don't need to loop over all messages
    };

};

void MessageHandler::draw(TCODConsole* console)
{
    //go through the latest messages and draw them until there's no more room
    std::vector<std::string> string_list = this->PrerenderMessages(10);
    std::vector<std::string>::reverse_iterator it = string_list.rbegin();
    int x = 0;
    int y = 0;
    TCODColor default_color = console->getDefaultForeground();
    for (it; it != string_list.rend(); ++it)
    {
        float coef = ((float)y)/10.0f;
        TCODColor new_color = TCODColor::lerp(default_color, TCODColor::darkGrey, coef);
        console->setDefaultForeground(new_color);
            console->print(x, y, (it)->c_str());
         y++;

        if (y > 10) break; //don't need to loop over all messages
    };

};

//goes through all the vectors of this turn and creates a std::string of the
//ones that share a turn
std::vector<std::string> MessageHandler::PrerenderMessages(int turn_limit)
{
    // if (Game::turn_count < turn_limit) 
    // {
    //     turn_limit = Game::turn_count;
    // }
    std::vector<std::string> prerendered_msgs = std::vector<std::string>();
    std::vector<Message*>::iterator it = this->msg_list.begin();

    std::string prerendered_single = "";
    int current_turn = Game::turn_count;
    bool appended_to = false;
    for (it; it != this->msg_list.end(); ++it)
    {
        if ((*it)->turn != current_turn) 
        {
            current_turn--;
            prerendered_msgs.push_back(prerendered_single);
            prerendered_single.clear();
            appended_to = true;
        }
        else
        {
        prerendered_single.append((*it)->content);
        appended_to = false;
        };

        // if (current_turn <= turn_limit) { break; }
    };
    if (!appended_to)
    {
    prerendered_msgs.push_back(prerendered_single);
    }
    return prerendered_msgs;
}

void Message::Init()
{
    this->content = "Unspecified %s";
    // this->vlist = "content";
    this->count = 0;
    this->turn = Game::turn_count;
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

    va_end(ap);

    handler->new_msg(this);
    this->turn = handler->game->turn_count;
};

