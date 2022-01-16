#ifndef POSITION_H
#define POSITION_H

#include "direction.h"

#include "utility"

using coordinate_t = int;

class Position {
    using pos_coords_t = std::pair<coordinate_t, coordinate_t>;
    pos_coords_t coordinates;
    Direction direction;

public:

    pos_coords_t get_coords() const {
        return coordinates;
    }

    Direction get_direction() const {
        return direction;
    }

    void set(pos_coords_t new_coordinates, Direction new_direction) {
        coordinates = new_coordinates;
        direction = new_direction;
    }

    void rotate_left() {
        int tmp = static_cast<int>(direction);
        tmp -= 1;
        if (tmp < 0)
            tmp += 4;
        direction = static_cast<Direction>(tmp);
    }

    void rotate_right() {
        int tmp = static_cast<int>(direction);
        tmp += 1;
        tmp %= 4;
        direction = static_cast<Direction>(tmp);
    }

    void move_forward() {
        switch (direction) {
            case WEST:
                coordinates.first--;
                break;
            case NORTH:
                coordinates.second++;
                break;
            case EAST:
                coordinates.first++;
                break;
            case SOUTH:
                coordinates.second--;
                break;
        }
    }

    void move_backward() {
        switch (direction) {
            case WEST:
                coordinates.first++;
                break;
            case NORTH:
                coordinates.second--;
                break;
            case EAST:
                coordinates.first--;
                break;
            case SOUTH:
                coordinates.second++;
                break;
        }
    }

    friend std::ostream &operator<<(std::ostream &os, const Position &rover);
};

std::ostream &operator<<(std::ostream &os, const Position &position);


#endif