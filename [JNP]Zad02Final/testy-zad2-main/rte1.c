#include "../maptel.h"

int main() {
	unsigned long id_del = maptel_create();
	maptel_delete(id_del + 1);
}
