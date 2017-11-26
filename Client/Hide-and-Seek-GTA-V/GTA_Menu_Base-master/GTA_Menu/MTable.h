#ifndef MTABLE_H
#define MTABLE_H

struct sub_table {
	char names[20][15];
	int count;
	const int get(const char* a);
};

#endif // !MTABLE_H