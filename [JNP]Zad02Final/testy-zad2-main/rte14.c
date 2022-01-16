#include "../maptel.h"

int main() {
	unsigned long id = maptel_create();
	char too_long[] = "123456789123456789123456789123456789";
	char tel_dst[TEL_NUM_MAX_LEN + 1];
	maptel_transform(id, too_long, tel_dst, TEL_NUM_MAX_LEN + 1);
}
