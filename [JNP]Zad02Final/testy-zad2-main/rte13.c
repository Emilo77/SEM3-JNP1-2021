#include "../maptel.h"

int main() {
	unsigned long id = maptel_create();
	char too_long[] = "123456789123456789123456789123456789";
	maptel_erase(id, too_long);
}
