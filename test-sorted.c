#if 0
#!/bin/bash
clear
gcc -o test-sorted test-sorted.c ref.c -Wall -Wfatal-errors -O3
if [ "$?" = "0" ]; then
	time ./test-sorted
fi
exit
#endif

#include "sorted.h"

sorted_type(double);

sorted_methods(double);

int double_Compare(const double *a, const double *b)
{
	double aVal = *a;
	double bVal = *b;
	if (aVal > bVal) return 1;
	if (aVal < bVal) return -1;
	return 0;
}

void test_double_sorted_1(void)
{
	double_sorted *list = double_sorted_New(10, double_Compare);
	double_sorted_PushValue(list, 5);
	assert(list->items.ptr[0] == 5);
	double_sorted_PushValue(list, 4);
	assert(list->items.ptr[1] == 4);
	assert(list->sortedindices.ptr[0] == 1);
	assert(list->sortedindices.ptr[1] == 0);
	gcSet(list, NULL);
}

int main(int argc, char *argv[])
{
	int i;
	int end = 1 << 0; // 25;
	for (i = 0; i < end; i++) {
		test_double_sorted_1();
	}
	return 0;
}