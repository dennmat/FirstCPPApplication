#ifndef MESSAGES_H
#define MESSAGES_H

#include <string>
#include <vector>
#include <libtcod.hpp>

class Message;
class Game;

class MessageHandler
{
    public:
        Game* game;
        std::vector<Message*> msg_list;
        void draw(TCODConsole* console);
        void new_msg(Message* message);
        MessageHandler();


};

class Message
{
    public:
        std::string content;
        // va_list vlist;
        int count;
        double long turn;

        Message();
        Message(MessageHandler* handler, std::string content, ...);
        void Init();


};

#endif
