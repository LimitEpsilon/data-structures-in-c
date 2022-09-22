#include "utils.h"

void failwith(const char *msg)
{
	fprintf(stderr, "%s\n", msg);
	exit(1);
}
