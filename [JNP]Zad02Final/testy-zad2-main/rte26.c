#include "../maptel.h"
#include <string.h>

int main() {
	unsigned long id = maptel_create();
	maptel_insert(id, "1", "");
}
