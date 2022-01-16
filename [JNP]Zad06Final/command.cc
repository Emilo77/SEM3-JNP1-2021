#include "command.h"

std::shared_ptr<Command> move_forward() {
    return std::make_shared<MoveForward>();
}

std::shared_ptr<Command> move_backward() {
    return std::make_shared<MoveBackward>();
}

std::shared_ptr<Command> rotate_right() {
    return std::make_shared<RotateRight>();
}

std::shared_ptr<Command> rotate_left() {
    return std::make_shared<RotateLeft>();
}

std::shared_ptr<Command> compose(std::initializer_list<std::shared_ptr<Command>>
                                 commands) {
    return std::make_shared<Compose>(commands);
}