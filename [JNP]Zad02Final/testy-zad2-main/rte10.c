#include "../maptel.h"

int main() {
	unsigned long id = maptel_create();
	maptel_transform(id, "0", NULL, TEL_NUM_MAX_LEN + 1);
}
