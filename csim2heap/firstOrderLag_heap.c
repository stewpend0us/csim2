#include "firstOrderLag_heap.h"

struct StrictlyProperBlock * firstOrderLag_new(size_t const numBlocks, double * const tau)
{
	struct StrictlyProperBlock stackb = firstOrderLag(numBlocks, tau);
	struct StrictlyProperBlock * heapb = malloc(sizeof(struct StrictlyProperBlock));
	memcpy(heapb, &stackb, sizeof(struct StrictlyProperBlock));
	return heapb;
}

void firstOrderLag_free(struct StrictlyProperBlock * b)
{
	free(b);
}