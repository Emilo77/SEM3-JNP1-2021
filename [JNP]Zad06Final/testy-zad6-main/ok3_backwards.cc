#include <cassert>
#include <iostream>
#include <memory>
#include <sstream>
#include "../rover.h"

struct FalseSensor : public Sensor {
    bool is_safe([[maybe_unused]] coordinate_t x,
                 [[maybe_unused]] coordinate_t y) override {
        return false;
    }
};

void assert_equal_output(const auto &rover, std::string_view output) {
	std::stringstream s;
	s << rover;
	assert(s.str() == output);
}

int main() {
	auto rover = RoverBuilder()
            .program_command('B', move_backward())
			.add_sensor(std::make_unique<FalseSensor>())
			.build();
	rover.land({0, 0}, Direction::NORTH);
	rover.execute("B");
	assert_equal_output(rover, "(0, 0) NORTH stopped");
}
