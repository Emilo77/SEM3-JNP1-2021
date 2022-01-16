#include <cassert>
#include <iostream>
#include <memory>
#include <sstream>
#include "../rover.h"

void assert_equal_output(const auto &rover, std::string_view output) {
	std::stringstream s;
	s << rover;
	assert(s.str() == output);
}

int main() {
	auto rover = RoverBuilder()
            .program_command('F', move_backward())
            .program_command('F', move_forward())
			.build();
	rover.land({0, 0}, Direction::EAST);
	rover.execute("F");
	assert_equal_output(rover, "(1, 0) EAST");
}
