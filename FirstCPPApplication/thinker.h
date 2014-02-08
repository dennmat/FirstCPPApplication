#pragma once
#ifndef THINKER_H
#define THINKER_H


class Game;
class Actor;

//class to handle the ai, from combat to movement
class Thinker
{


    public:
    Actor* master;
        void update();
        Thinker();
        ~Thinker();

};
#endif
