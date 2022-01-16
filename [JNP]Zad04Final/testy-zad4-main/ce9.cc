#include "../treasure_hunt.h"

constexpr void f() {
	Treasure<int, false> tre(42);
	tre.isTrapped = true;
}

int main() {
	f();
}
