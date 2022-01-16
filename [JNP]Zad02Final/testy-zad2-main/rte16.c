#include "../maptel.h"

int main() {
	unsigned long id = maptel_create();
	char invalid_character[] = "0a0";
	maptel_insert(id, invalid_character, "0");
}
