#include "stdafx.h"
#include <iostream>

#include "Room.h"

Room::Room()
{
    this->width = -1;
    this->height = -1;

    this->x = -1;
    this->y = -1;

    this->_door_index_limit = 3;
    this->_current_door_index = 0;
};

Room::Room(int x, int y, int width, int height, int door_index)
{

    this->width = width;
    this->height = height;

    this->x = x;
    this->y = y;

    this->_door_index_limit = door_index;
    this->_current_door_index = 0;
};

bool Room::checkDoorCount()
{

    this->_current_door_index++;

    if (this->_current_door_index == this->_door_index_limit )
    { 
        return true;
    }
    else
    {
        // std::cout << this->_current_door_index << " limit: " << this->_door_index_limit << std::endl;
        return false;
    }
};

bool Room::isPerimeter(int point_x, int point_y)
{
    if (point_x == 0 || point_x == width-1 ||
            point_y == 0 || point_y == height-1)
    {
        return true;
    }
    return false;
};
