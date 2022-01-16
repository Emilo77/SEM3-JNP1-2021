#ifndef COMMAND_H
#define COMMAND_H

#include "sensor.h"

#include <vector>
#include <initializer_list>
#include <memory>
#include <algorithm>

class Command {
protected:

    bool check_safety(Position &unchecked_new_position,
                      const std::vector<std::unique_ptr<Sensor>> &sensors) const {
        return std::all_of
                (
                        sensors.begin(),
                        sensors.end(),
                        [&](const std::unique_ptr<Sensor> &sensor) {
                            const auto&[x, y] = unchecked_new_position.get_coords();
                            return sensor->is_safe(x, y);
                        }
                );
    }

    void check_and_possibly_set(Position &position,
                                Position &unchecked_new_position,
                                const std::vector<std::unique_ptr<Sensor>>
                                &sensors) const {
        bool is_safe = check_safety(unchecked_new_position, sensors);
        if (is_safe)
            position = unchecked_new_position;
        else
            throw BrokenRover();
    }

public:

    virtual ~Command() {}

    virtual void execute([[maybe_unused]] Position &position, [[maybe_unused]]
    const std::vector<std::unique_ptr<Sensor>> &sensors) const {};

};

class Compose : public Command {
private:
    std::vector<std::shared_ptr<Command>> components;

public:
    Compose(std::initializer_list<std::shared_ptr<Command>> _components)
            : components(_components) {}

    void execute(Position &position, const std::vector<std::unique_ptr<Sensor>>
    &sensors) const override {
        std::for_each(
                components.begin(),
                components.end(),
                [&](const std::shared_ptr<Command> &c) {
                    c->execute(position, sensors);
                }
        );

    }
};

class RotateRight : public Command {
public:
    void execute(Position &position, const std::vector<std::unique_ptr<Sensor>>
    &sensors) const override {
        Position unchecked_new_position = position;
        unchecked_new_position.rotate_right();
        check_and_possibly_set(position, unchecked_new_position, sensors);
    }
};

class RotateLeft : public Command {
public:
    void execute(Position &position, const std::vector<std::unique_ptr<Sensor>>
    &sensors) const override {
        Position unchecked_new_position = position;
        unchecked_new_position.rotate_left();
        check_and_possibly_set(position, unchecked_new_position, sensors);
    }
};

class MoveForward : public Command {
public:
    void execute(Position &position, const std::vector<std::unique_ptr<Sensor>>
    &sensors) const override {
        Position unchecked_new_position = position;
        unchecked_new_position.move_forward();
        check_and_possibly_set(position, unchecked_new_position, sensors);
    }
};

class MoveBackward : public Command {
public:
    void execute(Position &position, const std::vector<std::unique_ptr<Sensor>>
    &sensors) const override {
        Position unchecked_new_position = position;
        unchecked_new_position.move_backward();
        check_and_possibly_set(position, unchecked_new_position, sensors);
    }
};

std::shared_ptr<Command> move_forward();

std::shared_ptr<Command> move_backward();

std::shared_ptr<Command> rotate_right();

std::shared_ptr<Command> rotate_left();

std::shared_ptr<Command>
compose(std::initializer_list<std::shared_ptr<Command>>);

#endif