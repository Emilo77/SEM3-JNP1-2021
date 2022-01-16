#include "../maptel.h"

int main() {
	unsigned long id = maptel_create();
	maptel_insert(id, "1234", "56789");
	char dst_len[0];
	maptel_transform(id, "1234", dst_len, 0);
}
