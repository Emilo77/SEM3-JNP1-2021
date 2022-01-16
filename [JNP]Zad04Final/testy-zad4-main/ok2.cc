#include "../treasure_hunt.h"

int main() {
	constexpr Adventurer<int, false> nieuzbroj;
	constexpr Adventurer<int, true> uzbroj(42);
	static_assert(uzbroj.getStrength() == 42);
	static_assert(nieuzbroj.isArmed == false);
	static_assert(uzbroj.isArmed == true);
}
