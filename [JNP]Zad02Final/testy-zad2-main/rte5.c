#include "../maptel.h"

int main() {
	unsigned long id_del = maptel_create();
	maptel_delete(id_del);
	char tel_dst[TEL_NUM_MAX_LEN + 1];
	maptel_transform(id_del, "0", tel_dst, TEL_NUM_MAX_LEN + 1);
}
