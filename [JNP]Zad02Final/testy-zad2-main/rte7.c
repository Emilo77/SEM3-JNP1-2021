#include "../maptel.h"

int main() {
	unsigned long id = maptel_create();
	maptel_insert(id, NULL, "0");
}
