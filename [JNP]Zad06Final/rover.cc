#include "rover.h"

[[maybe_unused]]
std::ostream &operator<<(std::ostream &os, const Rover &rover) {
    if (!rover.has_landed)
        os << "unknown";
    else {
        os << rover.position;
        if (rover.has_stopped)
            os << " stopped";
    }


    return os;
}