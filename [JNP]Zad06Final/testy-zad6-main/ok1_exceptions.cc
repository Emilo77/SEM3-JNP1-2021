#include <cassert>
#include <sstream>
#include "../rover.h"

void assert_equal_output(const auto &rover, std::string_view output) {
	std::stringstream s;
	s << rover;
	assert(s.str() == output);
}


int main() {
    auto rover = RoverBuilder().build();
    rover.land({0, 0}, Direction::EAST);
	try {
		rover.execute("A");
		rover.land({1, 0}, Direction::NORTH);
	}
	catch (...) {
        assert(false);
	}
	assert_equal_output(rover, "(1, 0) NORTH");
}
