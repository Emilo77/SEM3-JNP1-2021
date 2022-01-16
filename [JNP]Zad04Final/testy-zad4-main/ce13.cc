#include "../treasure_hunt.h"

#include <cstddef>

struct Dummy {
	bool isArmed = true;
	int pay() { return 0; }
	template<bool IsTrapped> void loot(Treasure<int, IsTrapped>) {}
};

constexpr void f() {
	Treasure<int, false> t(42);
	Dummy d;
	run(Encounter{t, d});
}

int main() {
	f();
}
