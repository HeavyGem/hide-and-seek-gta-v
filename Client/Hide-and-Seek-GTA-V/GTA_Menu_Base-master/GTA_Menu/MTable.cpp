#include "MTable.h"
#include "Includes.h"

const int sub_table::get(const char* a) {
	for (int i = 0; i < sizeof(names) / sizeof(names[0]); i++) {
		if (!strcmp(names[i], a)) return i;
	}
	strcpy(names[count++], a);
	return count - 1;
}