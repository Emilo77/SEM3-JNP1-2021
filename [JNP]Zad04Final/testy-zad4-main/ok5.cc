#include "../treasure_hunt.h"

#include <cassert>

constexpr bool is_ok() {
	Veteran<int, 0>();
	Veteran<int, 1>();
	Veteran<int, 2>();
	Veteran<int, 3>();
	Veteran<int, 4>();
	Veteran<int, 5>();
	Veteran<int, 6>();
	Veteran<int, 7>();
	Veteran<int, 8>();
	Veteran<int, 9>();
	Veteran<int, 10>();
	Veteran<int, 11>();
	Veteran<int, 12>();
	Veteran<int, 13>();
	Veteran<int, 14>();
	Veteran<int, 15>();
	Veteran<int, 16>();
	Veteran<int, 17>();
	Veteran<int, 18>();
	Veteran<int, 19>();
	Veteran<int, 20>();
	Veteran<int, 21>();
	Veteran<int, 22>();
	Veteran<int, 23>();
	Veteran<int, 24>();
	static_assert(Veteran<int, 24>().getStrength() == 46368);
	static_assert(Veteran<int, 24>::isArmed == true);

	Veteran<int, 24> vet;
	Treasure<int, true> tre(42);
	expedition(Encounter{vet, tre});
	if(tre.evaluate() != 0)
		return false;
	if(vet.pay() != 42)
		return false;
	if(vet.pay() != 0)
		return false;
	return true;
}

int main() {
	static_assert(is_ok());
}
