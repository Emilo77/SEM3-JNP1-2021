#include "cpp_helper_functions.h"
#include "../maptel.h"

int main() {
	constexpr int n = int(1e5);
	for(int i = 0; i < n; ++i)
		jnp1::maptel_create();
}
