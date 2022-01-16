#include <cassert>
#include <iostream>
#include <sstream>
#include <memory>

#include "../rover.h"

void assert_equal_output(const auto &rover, std::string_view output) {
	std::stringstream s;
	s << rover;
	assert(s.str() == output);
}

struct SometimesWorkingSensor : public Sensor {
    bool is_safe([[maybe_unused]] coordinate_t x,
                 [[maybe_unused]] coordinate_t y) override {
        if (working >= 2) {
            working = 0;
            return false;
        }
        working++;
        return true;
    }
    SometimesWorkingSensor(): working(0) {}
private:
    int working;
};

int main() {
    // Podczas budowy łazika można zaprogramawać jego komendy oraz ustawić różne czujniki.
    auto rover = RoverBuilder()
            .program_command('U', compose({
                        move_forward(),
                        move_forward(),
                        move_forward(), // Tu powinien się zepsuć
                        rotate_right(),
                        move_forward()
                        }))
            .add_sensor(std::make_unique<SometimesWorkingSensor>())
            .build();

    // Przed lądowaniem nie jest możliwe sterowanie łazikiem.
    try {
        rover.execute("U");
        assert(false);
    } catch (std::exception const& e) {
    }

    rover.land({0, 0}, Direction::EAST);
    assert_equal_output(rover, "(0, 0) EAST");
    rover.execute("U");
    assert_equal_output(rover, "(2, 0) EAST stopped");
    rover.execute("U");
    assert_equal_output(rover, "(4, 0) EAST stopped");
}
