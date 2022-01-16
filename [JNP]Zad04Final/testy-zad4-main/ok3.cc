#include "../treasure_hunt.h"
#include <cassert>

#define return_false_if_not(b) if(!b) return false

constexpr bool is_ok() { 
	Adventurer<int, true> uzbroj(42);
	{
		Treasure<int, false> bez_pulapki(42);
		run(Encounter{uzbroj, bez_pulapki});
		return_false_if_not(uzbroj.pay() == 42);
		return_false_if_not(bez_pulapki.evaluate() == 0);
		return_false_if_not(uzbroj.getStrength() == 42);
	}

	for(int i = 0; i < 6; ++i) {
		Treasure<int, true> pulapka(42);
		run(Encounter{uzbroj, pulapka});
		return_false_if_not(uzbroj.pay() == 42);
		return_false_if_not(pulapka.evaluate() == 0);
	}

	{
		Treasure<int, true> pulapka(42);
		run(Encounter{uzbroj, pulapka});
		return_false_if_not(uzbroj.pay() == 0);
		return_false_if_not(pulapka.evaluate() == 42);
	}

	{
		Treasure<int, false> bez_pulapki(42);
		run(Encounter{uzbroj, bez_pulapki});
		return_false_if_not(uzbroj.pay() == 42);
		return_false_if_not(bez_pulapki.evaluate() == 0);
		return_false_if_not(uzbroj.getStrength() == 0);
	}

	Adventurer<int, false> nieuzbroj;
	{
		Treasure<int, false> bez_pulapki(42);
		run(Encounter{nieuzbroj, bez_pulapki});
		return_false_if_not(nieuzbroj.pay() == 42);
		return_false_if_not(bez_pulapki.evaluate() == 0);
	}

	{
		Treasure<int, true> pulapka(42);
		run(Encounter{nieuzbroj, pulapka});
		return_false_if_not(nieuzbroj.pay() == 0);
		return_false_if_not(pulapka.evaluate() == 42);
	}
	return true;
}

int main() {
	static_assert(is_ok());
}
