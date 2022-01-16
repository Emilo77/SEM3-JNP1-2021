#include "../treasure_hunt.h"

constexpr void f() {
	Treasure<int, false> t1(42);
	Treasure<int, false> t2(42);
	run(Encounter{t1, t2});
}

int main() {
	f();
}
