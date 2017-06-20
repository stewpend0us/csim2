#include <string.h>
#include "secondOrderSystem_heap.h"

struct StrictlyProperBlock * secondOrderSystem_new(size_t const numBlocks, struct secondOrderSystemStorage * const storage)
{
	struct StrictlyProperBlock stackb = secondOrderSystem(numBlocks, storage);
	struct StrictlyProperBlock * heapb = malloc(sizeof(struct StrictlyProperBlock));
	if (heapb)
		memcpy(heapb, &stackb, sizeof(struct StrictlyProperBlock));
	return heapb;
}

void secondOrderSystem_free(struct StrictlyProperBlock * b)
{
	free(b);
}