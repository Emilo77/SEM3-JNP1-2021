#include "../treasure_hunt.h"
#include <cassert>

constexpr bool is_ok() {
	Veteran<int, 10>::strength_t s = 0;
	return (s - 1) == 4294967295;
}

int main() {
	static_assert(is_ok());
}
