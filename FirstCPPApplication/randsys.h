#pragma once
#ifndef RANDSYS_H
#define RANDSYS_H
#include <map>
#include "enums\spawntypes_t.h"


class TCODRandom;
//basically a dictionary that'll let you add an item with a specific weighted
//score, so I can simplify drops and stuff
template<typename T>
class RandomWeightMap
{
    private:
        std::map<int, T> _map;

    public:
        void add_item(int weight, T item);
        T get_item(int score);
        T get_item(TCODRandom* rng);

        int get_total_weight();
        RandomWeightMap();
};
    template<typename T>
RandomWeightMap<T>::RandomWeightMap()
{
    this->_map = std::map<int, T>();
};

template<typename T>
void RandomWeightMap<T>::add_item(int weight, T item)
{
    std::pair<int, T> pair = std::make_pair<int, T>(weight, item);
    this->_map.insert(pair);
};

template<typename T>
T RandomWeightMap<T>::get_item(TCODRandom* rng)
{
    int total_weight = this->get_total_weight();
    return this->get_item(rng->getInt(0, total_weight));
};

template<typename T>
T RandomWeightMap<T>::get_item(int score)
{
    int total_weight = this->get_total_weight();
    int cumulative_weight = 0;
    for (auto it = this->_map.begin(); it != this->_map.end(); it++)
    {
        cumulative_weight+=it->first;
        if (score < cumulative_weight)
        {
            return it->second;
        };
    };

    //assert(false && "score: " << score << " out of range: " << total_weight);
};

template<typename T>
int RandomWeightMap<T>::get_total_weight()
{
    int total_weight = 0;
    for (auto it = this->_map.begin(); it != this->_map.end(); it++)
    {
        total_weight+=it->first;
    };
    return total_weight;
};
#endif
