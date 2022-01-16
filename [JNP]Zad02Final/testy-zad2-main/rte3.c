#include "../maptel.h"

int main() {
	unsigned long id_del = maptel_create();
	maptel_delete(id_del);
	maptel_insert(id_del, "0", "0");
}
