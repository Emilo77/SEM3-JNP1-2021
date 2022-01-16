#ifndef ROVER_H
#define ROVER_H

#include "sensor.h"
#include "command.h"
#include "position.h"
#include "rover_exceptions.h"

#include <iostream>
#include <string>
#include <memory>
#include <map>

class Rover {
private:
    Position position;
    std::map<char, std::shared_ptr<Command>> commands;
    std::vector<std::unique_ptr<Sensor>> sensors;
    bool has_stopped;
    bool has_landed;

    Rover(std::map<char, std::shared_ptr<Command>> _commands,
          std::vector<std::unique_ptr<Sensor>> _sensors)
            : commands(std::move(_commands)), sensors(std::move(_sensors)),
              has_stopped(false), has_landed(false) {};

public:

    void land(std::pair<coordinate_t, coordinate_t> _position,
              Direction _direction) {
        position.set(_position, _direction);
        has_landed = true;
        has_stopped = false;
    }

    void execute(const std::string &s) {
        if (!has_landed)
            throw RoverNotLanded();

        for (const char c: s) {
            auto it = commands.find(c);
            if (it == commands.end()) {
                has_stopped = true;
                return;
            } else {
                try {
                    it->second->execute(position, sensors);
                    has_stopped = false;
                }
                catch (const BrokenRover &br) {
                    has_stopped = true;
                    return;
                }
            }
        }

    }

    friend std::ostream &operator<<(std::ostream &os, const Rover &rover);

    friend class RoverBuilder;
};

std::ostream &operator<<(std::ostream &os, const Rover &rover);

class RoverBuilder {
private:
    std::map<char, std::shared_ptr<Command>> commands;
    std::vector<std::unique_ptr<Sensor>> sensors;

public:

    RoverBuilder() = default;

    RoverBuilder(const RoverBuilder &&other) = delete;

    RoverBuilder(RoverBuilder &&other) = delete;

    RoverBuilder &program_command(char name, std::shared_ptr<Command> command) {
        commands[name] = command;
        return *this;
    }

    RoverBuilder &add_sensor(std::unique_ptr<Sensor> sensor) {
        sensors.emplace_back(std::move(sensor));
        return *this;
    }

    Rover build() {
        return Rover(std::move(commands), std::move(sensors));
    }
};

#endif