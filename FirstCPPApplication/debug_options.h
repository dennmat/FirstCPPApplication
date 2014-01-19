#ifndef DEBUG_OPTIONS_H
#define DEBUG_OPTIONS_H

class Game;

class DebugOptions
{
    public:
        bool all_vision;

        DebugOptions() 
        {
            all_vision = true;
        };
};

#endif
