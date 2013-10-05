#pragma once
#ifndef THINKER_H
#define THINKER_H


class Actor;

//class to handle the ai, from combat to movement
class Thinker
{

    Actor* master;

    public:
        void update();
        Thinker();

};
#endif
