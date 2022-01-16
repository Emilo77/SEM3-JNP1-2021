#include "cpp_helper_functions.h"
#include "../maptel.h"

int main() {
	constexpr int n = int(1e2);
	unsigned long id = jnp1::maptel_create();
	for(int i = 0; i < n - 1; ++i)
		insert(id, i, i + 1);

	constexpr int q = int(1e4);
	for(int iter = 0; iter < q; ++iter) {
		int i = rd(0, n - 2);
		int j = rd(0, n - 1);
		insert(id, i, j);
		assert_transform_is_equal(id, i, (i < j ? n - 1 : i));
		insert(id, i, i + 1);
		assert_transform_is_equal(id, i, n - 1);
	}
}
