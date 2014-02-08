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
        void draw_raw(TCODConsole* console);
        void new_msg(Message* message);
        MessageHandler();
        std::vector<std::string> PrerenderMessages(int turn_limit);


};

class Message
{
    public:
        std::string content;
        int count;
        double long turn;

        Message();
        Message(MessageHandler* handler, std::string content, ...);
        void Init();


};

#endif
