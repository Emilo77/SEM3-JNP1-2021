#include "../maptel.h"

int main() {
	unsigned long id = maptel_create();
	maptel_insert(id, "0", NULL);
}
