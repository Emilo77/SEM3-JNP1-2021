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
	maptel_insert(id, "0", "5");
	maptel_insert(id, "1", "8");
	maptel_insert(id, "2", "6");
	maptel_insert(id, "3", "4");
	maptel_insert(id, "4", "8");
	maptel_insert(id, "5", "4");
	maptel_insert(id, "6", "5");
	maptel_insert(id, "7", "6");
	maptel_insert(id, "8", "3");
	assert_transform_is_equal(id, "0", "0");
	assert_transform_is_equal(id, "1", "1");
	assert_transform_is_equal(id, "2", "2");
	assert_transform_is_equal(id, "3", "3");
	assert_transform_is_equal(id, "4", "4");
	assert_transform_is_equal(id, "5", "5");
	assert_transform_is_equal(id, "6", "6");
	assert_transform_is_equal(id, "7", "7");
	assert_transform_is_equal(id, "8", "8");
	assert_transform_is_equal(id, "9", "9");
}
