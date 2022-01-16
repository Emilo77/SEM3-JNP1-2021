#ifndef ROVER_EXCEPTIONS_H
#define ROVER_EXCEPTIONS_H

#include <stdexcept>
#include <string>

class BrokenRover : public std::runtime_error {
public:
    BrokenRover(const std::string &what = "")
            : std::runtime_error(what) {}
};

class RoverNotLanded : public std::runtime_error {
public:
    RoverNotLanded(const std::string &what = "")
            : std::runtime_error(what) {}
};

#endif