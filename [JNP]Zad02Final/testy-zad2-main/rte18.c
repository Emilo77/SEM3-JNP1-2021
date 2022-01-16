#include "../maptel.h"

int main() {
	unsigned long id = maptel_create();
	char invalid_character[] = "0a0";
	char tel_dst[TEL_NUM_MAX_LEN + 1];
	maptel_transform(id, invalid_character, tel_dst, TEL_NUM_MAX_LEN + 1);
}
