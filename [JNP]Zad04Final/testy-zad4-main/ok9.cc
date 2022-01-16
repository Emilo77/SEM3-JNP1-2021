#include "../treasure_hunt.h"

#include <cassert>

template<class A, class B>
constexpr bool check_whether_left_winner(A a, B b) {
	Treasure<int, false> t1(1);
	Treasure<int, false> t2(2);
	run(Encounter{a, t1});
	run(Encounter{b, t2});
	run(Encounter{a, b});
	if(a.pay() != 3 or b.pay() != 0)
		return false;
	Treasure<int, false> t3(4);
	run(Encounter{b, t3});
	run(Encounter{b, a});
	return a.pay() == 4 and b.pay() == 0;
}

template<class A, class B>
constexpr bool check_whether_draw(A a, B b) {
	Treasure<int, false> t1(1);
	Treasure<int, false> t2(2);
	run(Encounter{a, t1});
	run(Encounter{b, t2});
	run(Encounter{a, b});
	return a.pay() == 1 and b.pay() == 2;
}

constexpr bool is_ok() {
	if(not check_whether_left_winner<>(Veteran<int, 4>(), Veteran<int, 3>()))
		return false;
	if(not check_whether_draw<>(Veteran<int, 4>(), Veteran<int, 4>()))
		return false;

	if(not check_whether_draw<>(Explorer<int>(), Explorer<int>()))
		return false;

	if(not check_whether_left_winner<>(Adventurer<int, true>(4), Adventurer<int, true>(3)))
		return false;
	if(not check_whether_draw<>(Adventurer<int, true>(3), Adventurer<int, true>(3)))
		return false;

	if(not check_whether_left_winner<>(Veteran<int, 0>(), Explorer<int>()))
		return false;
	if(not check_whether_left_winner<>(Veteran<int, 6>(), Adventurer<int, true>(6)))
		return false;
	if(not check_whether_left_winner<>(Adventurer<int, true>(100), Veteran<int, 5>()))
		return false;
	if(not check_whether_draw<>(Veteran<int, 5>(), Adventurer<int, true>(5)))
		return false;

	if(not check_whether_left_winner<>(Adventurer<int, true>(0), Explorer<int>()))
		return false;

	return true;
}

int main() {
	static_assert(is_ok());
}
