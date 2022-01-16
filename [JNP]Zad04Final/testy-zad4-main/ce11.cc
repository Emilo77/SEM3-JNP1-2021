#include "../treasure_hunt.h"

constexpr void f() {
	Veteran<int, 1> vet;
	Treasure<long, false> tre(42);
	expedition(Encounter{vet, tre});
}

int main() {
	f();
}
