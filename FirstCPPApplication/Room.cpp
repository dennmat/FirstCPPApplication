#include "stdafx.h"
#include <iostream>

#include "Room.h"

Room::Room()
{
    this->width = -1;
    this->height = -1;

    this->x = -1;
    this->y = -1;

    this->_door_number_limit = 3;
    this->_current_door_number = 0;
};

Room::Room(int x, int y, int width, int height, int door_number)
{

    this->width = width;
    this->height = height;

    this->x = x;
    this->y = y;

    this->_door_number_limit = door_number;
    this->_current_door_number = 0;
};

bool Room::checkDoorCount()
{

    this->_current_door_number++;

    if (this->_current_door_number == this->_door_number_limit )
    { 
        return true;
    }
    else
    {
        std::cout << this->_current_door_number << " limit: " << this->_door_number_limit << std::endl;
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
