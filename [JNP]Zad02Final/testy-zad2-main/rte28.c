#include "../maptel.h"
#include <string.h>

int main() {
	unsigned long id = maptel_create();
	char tel_dst[TEL_NUM_MAX_LEN + 1];
	maptel_transform(id, "", tel_dst, TEL_NUM_MAX_LEN + 1);
}
