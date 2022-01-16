#ifndef DIRECTION_H
#define DIRECTION_H

#include <iostream>

enum Direction : int {
    WEST, NORTH, EAST, SOUTH
};

std::ostream &operator<<(std::ostream &os, const Direction &direction);


#endif