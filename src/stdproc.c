#include "../lib/stdproc.h"

void ksleep(int __ms)
{
	/* TODO: Make this less hacky */

	for (int __iptr = 0; __iptr < (__ms * KSLEEP_MULTIPLIER); __iptr++) {}
}
