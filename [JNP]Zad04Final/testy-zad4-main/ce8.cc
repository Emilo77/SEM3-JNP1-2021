#include "../treasure_hunt.h"

constexpr void f() {
	Adventurer<int, false> adv;
	adv.isArmed = true;
}

int main() {
	f();
}
