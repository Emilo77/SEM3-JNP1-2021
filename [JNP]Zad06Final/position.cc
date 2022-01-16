#include "position.h"

[[maybe_unused]]
std::ostream &operator<<(std::ostream &os, const Position &position) {
    os << "(" << position.coordinates.first << ", " <<
       position.coordinates.second << ") " << position.direction;
    return os;
}