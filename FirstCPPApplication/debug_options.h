#ifndef DEBUG_OPTIONS_H
#define DEBUG_OPTIONS_H

class Game;

class DebugOptions
{
    public:
        bool all_vision;
        bool show_msg_debug;

        DebugOptions() 
        {
            all_vision = false;
            show_msg_debug = false;
        };
};

#endif
