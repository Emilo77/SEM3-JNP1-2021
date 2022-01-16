#include "cpp_helper_functions.h"
#include "../maptel.h"

int main() {
	set_seed("big_lot_of_everything");
	constexpr int n = int(3e4);
	for(int i = 0; i < n; ++i) {
		unsigned long id = jnp1::maptel_create();
		int a = rd(0, 4), b = rd(0, 4);
		insert(id, a, b);
		assert_transform_is_equal(id, a, b);
		assert_transform_is_equal(id, b, b);
		if(rd(0, 1)) {
			jnp1::maptel_erase(id, to_string(a).c_str());
			assert_transform_is_equal(id, a, a);
			assert_transform_is_equal(id, b, b);
		}
		jnp1::maptel_delete(id);
	}
}
