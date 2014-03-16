#ifndef MESSAGES_H
#define MESSAGES_H

#include <string>
#include <vector>
#include <libtcod.hpp>

class Message;
class Game;

enum message_types_t {
    NOTYPE_MSG = 0,
    MOOD_MSG,
    TILE_DESCRIPTION_MSG,
    DAMAGE_GIVEN_MSG, DAMAGE_TAKEN_MSG,
    HELP_MSG, CHAT_MSG,
    HUNGER_MSG, BURDEN_MSG
};

class MessageHandler
{
    public:
        static message_types_t message_order [8];// ={}; // {HUNGER_MSG, BURDEN_MSG, MOOD_MSG, DAMAGE_GIVEN_MSG, DAMAGE_TAKEN_MSG, CHAT_MSG, TILE_DESCRIPTION_MSG, NOTYPE_MSG};
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

        message_types_t type;

        Message();
        Message(MessageHandler* handler, std::string content, ...);
        void Init();


};

#endif
