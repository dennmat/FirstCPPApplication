#pragma once
#ifndef RANDSYS_H
#define RANDSYS_H
#include <map>


class TCODRandom;
//basically a dictionary that'll let you add an item with a specific weighted
//score, so I can simplify drops and stuff
template<class T>
class RandomWeightMap
{
    private:
        std::map<int, T*> _map;

    public:
        void add_item(int weight, T* item);
        T* get_item(int score);
        T* get_item(TCODRandom* rng);

        int get_total_weight();
        RandomWeightMap();
};
#endif
