#include "../maptel.h"

int main() {
	unsigned long id = maptel_create();
	char too_long[] = "01234567890123456789012";
	maptel_insert(id, "0", too_long);
}
