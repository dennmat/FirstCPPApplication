#include "stdafx.h"
#include <iostream>
#include <sstream>
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
    // std::cout << "creating message handler" << std::endl;
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
    std::vector<std::string>::iterator it = string_list.begin();
    int x = 0;
    int y = 0;
    TCODColor default_color = console->getDefaultForeground();
    for (it; it != string_list.end(); ++it)
    {

        TCODColor  new_color;
        float coef = ((float)y)/10.0f;
        if (y == 0)
        {
            new_color = TCODColor::lightestRed;
        }
        else 
        {
            new_color = TCODColor::lerp(default_color, TCODColor::darkestGrey, coef);
        };
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
    std::vector<std::string> prerendered_msgs;
    int last_turn = Game::turn_count;
    int turn_count = 0;
    std::string prerendered_single = "";

    for (std::vector<Message*>::reverse_iterator it = this->msg_list.rbegin(); it != this->msg_list.rend(); ++it) {
        if (turn_count >= 10) break;

        if (last_turn != (*it)->turn) 
        {
            turn_count++;
            prerendered_msgs.push_back(prerendered_single);
            prerendered_single.clear();
            last_turn = (*it)->turn;
        };

        //convert turn number to string and append it
        std::stringstream ss;
        ss << (*it)->turn;
        std::string turn_str = ss.str();
        prerendered_single.append(((*it)->content+" "+ turn_str+" "));
    }

    if (prerendered_single.size() > 0) {
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

    char* x = new char[255*32];
    vsprintf(x, content.c_str(), ap);
    this->content = std::string(x);

    va_end(ap);

    handler->new_msg(this);
    this->turn = handler->game->turn_count;
};

