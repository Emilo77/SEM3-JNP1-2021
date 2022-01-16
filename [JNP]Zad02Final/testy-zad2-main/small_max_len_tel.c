#include "../maptel.h"

int main() {
	unsigned long id = maptel_create();
	char just_right[] = "0123456789012345678901";
	maptel_insert(id, "0", just_right);
}
