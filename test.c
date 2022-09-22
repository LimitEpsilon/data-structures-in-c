#include "src/utils.h"

int main(void)
{
	char *keys[] = { "a", "b", "c", "d", "e", "f", "g", "h", "i",
			 "j", "k", "l", "m", "n", "o", "p", "q", "r",
			 "s", "t", "u", "v", "w", "x", "y", "z" };
	int values[26];
	int i;
	for (i = 0; i < 26; ++i) {
		insert(keys[i], (void *)(values[i]));
	}
	print_table();
	return 0;
}
