#include "../treasure_hunt.h"

#include <cstddef>

consteval bool f() {
	Veteran<int, 23>();
	Veteran<int, 24>();
	Veteran<int, 25>();
	return true;
}

int main() {
	static_assert(f());
}
