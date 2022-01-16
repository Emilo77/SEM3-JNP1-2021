#include "../treasure_hunt.h"

#include <cassert>

template<bool IsTrapped>
constexpr bool check_ok_treasure(Treasure<int, IsTrapped> &t) {
	if(t.evaluate() != 42)
		return false;
	if(t.isTrapped != IsTrapped)
		return false;
	if(t.getLoot() != 42)
		return false;
	if(t.evaluate() != 0)
		return false;
	return true;
}

constexpr bool is_ok() {
	Treasure<int, false> t1(42);
	Treasure<int, true> t2(42);
	SafeTreasure<int> t3(42);
	TrappedTreasure<int> t4(42);
	if(not check_ok_treasure<false>(t1))
		return false;
	if(not check_ok_treasure<true>(t2))
		return false;
	if(not check_ok_treasure<false>(t3))
		return false;
	if(not check_ok_treasure<true>(t4))
		return false;
	return true;
}

int main() {
	static_assert(is_ok());
}
