#include "../maptel.h"
#include <assert.h>
#include <string.h>

void assert_transform_is_equal(unsigned long id, char from[], char to[]) {
	char tel_dst[TEL_NUM_MAX_LEN + 1];
	maptel_transform(id, from, tel_dst, TEL_NUM_MAX_LEN + 1);
	assert(strcmp(tel_dst, to) == 0);
}

int main() {
	unsigned long id = maptel_create();
	maptel_insert(id, "0", "2");
	maptel_insert(id, "1", "2");
	maptel_insert(id, "3", "5");
	maptel_insert(id, "4", "5");
	assert_transform_is_equal(id, "0", "2");
	assert_transform_is_equal(id, "1", "2");
	assert_transform_is_equal(id, "2", "2");
	assert_transform_is_equal(id, "3", "5");
	assert_transform_is_equal(id, "4", "5");
	assert_transform_is_equal(id, "5", "5");
}
