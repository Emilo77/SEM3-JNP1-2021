#include "cpp_helper_functions.h"
#include "../maptel.h"

int main() {
	set_seed("big_lot_of_inserts");
	unsigned long id = jnp1::maptel_create();
	constexpr int n = int(1e5);
	for(int i = 0; i < n; ++i)
		insert(id, random_tel(), random_tel());
}
