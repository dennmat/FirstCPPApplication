#include "stdafx.h"

#include "Room.h"

Room::Room()
{
    this->width = -1;
    this->height = -1;

    this->x = -1;
    this->y = -1;
};

Room::Room(int x, int y, int width, int height)
{
    this->width = width;
    this->height = height;

    this->x = x;
    this->y = y;
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
