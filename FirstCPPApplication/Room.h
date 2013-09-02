#ifndef ROOM_H
#define ROOM_H

class Room
{
    public:
        Room();
        ~Room();

        //w and h of the room
        int width, height;

        //its location on the map
        int x, y;

        bool Room::isPerimeter(int point_x, int point_y);

        
};

#endif
