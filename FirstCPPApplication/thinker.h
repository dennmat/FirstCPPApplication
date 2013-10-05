#pragma once
#ifndef THINKER_H
#define THINKER_H


class Actor;

//class to handle the ai, from combat to movement
class Thinker
{


    public:
    Actor* master;
        void update();
        Thinker();

};
#endif
