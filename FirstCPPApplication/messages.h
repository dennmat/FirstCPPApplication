#ifndef MESSAGES_H
#define MESSAGES_H

#include <string>
#include <vector>
#include <libtcod.hpp>

class Message;

class MessageHandler
{
    public:
    std::vector<Message*> msg_list;
    void draw(TCODConsole* console);
    MessageHandler();


};

class Message
{
    public:
        std::string content;
        va_list vlist;

        Message();
        Message(std::string content, ... );
        void Init();


};

#endif
