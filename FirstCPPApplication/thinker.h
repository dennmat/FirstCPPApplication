#pragma once
#ifndef THINKER_H
#define THINKER_H


class Game;
class Actor;

//class to handle the ai, from combat to movement
class Thinker
{
    private:
        int consecutive_fails;
        bool path_possible;
        int skipped_tries;
        int consecutive_skips;
        int skip_threshold;
        int fail_threshold;



    public:
        Actor* master;

        int is_dumb;
        int is_civ;
        int is_shopkeep;

        void update();
        void smart_update();
        void dumb_update();
        Thinker();
        ~Thinker();
        void walk_towards_player();
        void walk_dumbly();
        void try_attacking_player();

};
#endif
