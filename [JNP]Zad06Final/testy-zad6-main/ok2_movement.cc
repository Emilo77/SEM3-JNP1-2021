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
	{
		auto rover = RoverBuilder().build();
		rover.land({0, 0}, Direction::EAST);
		assert_equal_output(rover, "(0, 0) EAST");
	}
	{
		auto rover = RoverBuilder().build();
		rover.land({0, 0}, Direction::WEST);
		assert_equal_output(rover, "(0, 0) WEST");
	}
	{
		auto rover = RoverBuilder().build();
		rover.land({0, 0}, Direction::SOUTH);
		assert_equal_output(rover, "(0, 0) SOUTH");
	}
	{
		auto rover = RoverBuilder().build();
		rover.land({0, 0}, Direction::NORTH);
		assert_equal_output(rover, "(0, 0) NORTH");
	}

    auto rover = RoverBuilder()
            .program_command('F', move_forward())
            .program_command('B', move_backward())
            .program_command('R', rotate_right())
            .program_command('L', rotate_left())
            .build();
    rover.land({0, 0}, Direction::EAST);

	rover.execute("R");
	assert_equal_output(rover, "(0, 0) SOUTH");
	rover.execute("F");
	assert_equal_output(rover, "(0, -1) SOUTH");
	rover.execute("B");
	assert_equal_output(rover, "(0, 0) SOUTH");

	rover.execute("R");
	assert_equal_output(rover, "(0, 0) WEST");
	rover.execute("F");
	assert_equal_output(rover, "(-1, 0) WEST");
	rover.execute("B");
	assert_equal_output(rover, "(0, 0) WEST");

	rover.execute("R");
	assert_equal_output(rover, "(0, 0) NORTH");
	rover.execute("F");
	assert_equal_output(rover, "(0, 1) NORTH");
	rover.execute("B");
	assert_equal_output(rover, "(0, 0) NORTH");

	rover.execute("R");
	assert_equal_output(rover, "(0, 0) EAST");
	rover.execute("F");
	assert_equal_output(rover, "(1, 0) EAST");
	rover.execute("B");
	assert_equal_output(rover, "(0, 0) EAST");

	rover.execute("L");
	assert_equal_output(rover, "(0, 0) NORTH");
	rover.execute("L");
	assert_equal_output(rover, "(0, 0) WEST");
	rover.execute("L");
	assert_equal_output(rover, "(0, 0) SOUTH");
	rover.execute("L");
	assert_equal_output(rover, "(0, 0) EAST");
}
