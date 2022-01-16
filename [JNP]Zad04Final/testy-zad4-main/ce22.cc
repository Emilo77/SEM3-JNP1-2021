#include "../treasure_hunt.h"

#include <cstddef>

template<typename A, bool b>
struct Dummy {
	using Treasure = size_t;
	constexpr int getLoot() {
		int valueReturned = 41;
		return valueReturned;
	}
};

consteval bool f() {
	Adventurer<int, true> t(1);
	Dummy<int, false> d;
	run(Encounter{t, d});
	return true;
}

int main() {
	static_assert(f());
}
