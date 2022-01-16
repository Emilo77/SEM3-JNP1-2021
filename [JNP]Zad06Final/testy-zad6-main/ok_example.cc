#include <cassert>
#include <iostream>
#include <memory>
#include <sstream>
#include "../rover.h"

struct TrueSensor : public Sensor {
    bool is_safe([[maybe_unused]] coordinate_t x,
                 [[maybe_unused]] coordinate_t y) override {
        return true;
    }
};

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
    // Podczas budowy łazika można zaprogramawać jego komendy oraz ustawić różne czujniki.
    auto rover = RoverBuilder()
            .program_command('F', move_forward())
            .program_command('B', move_backward())
            .program_command('R', rotate_right())
            .program_command('L', rotate_left())
            .program_command('U', compose({rotate_right(), rotate_right()}))
            .add_sensor(std::make_unique<TrueSensor>())
            .add_sensor(std::make_unique<TrueSensor>())
            .build();

    // Przed lądowaniem nie jest możliwe sterowanie łazikiem.
	assert_equal_output(rover, "unknown");
    // std::cout << rover << std::endl;
    try {
        rover.execute("F");
        assert(false);
    } catch (std::exception const& e) {
    }

    // Po lądowaniu łazik wykonuje przesłane komendy.
    rover.land({0, 0}, Direction::EAST);
	assert_equal_output(rover, "(0, 0) EAST");
    rover.execute("FFBRLU");
	assert_equal_output(rover, "(1, 0) WEST");

    // Łazik zatrzymuje się, gdy napotka nieznaną komendę.
    rover.execute("FXFFF");
	assert_equal_output(rover, "(0, 0) WEST stopped");

    // Łazik wykonuje poprawne komendy.
    rover.execute("FFF");
	assert_equal_output(rover, "(-3, 0) WEST");

    // Łazik zatrzymuje się również, gdy czujniki zgłaszają niebezpieczeństwo.
    auto broken_rover = RoverBuilder()
            .program_command('X', move_forward())
            .add_sensor(std::make_unique<FalseSensor>())
            .build();
    broken_rover.land({-1, -1}, Direction::WEST);
    broken_rover.execute("X");
	assert_equal_output(broken_rover, "(-1, -1) WEST stopped");
}
