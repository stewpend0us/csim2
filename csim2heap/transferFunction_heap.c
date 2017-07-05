#include <string.h>
#include "transferFunction_heap.h"

struct StrictlyProperBlock * transferFunction_new(size_t const numBlocks, struct transferFunctionStorage * const storage)
{
	struct StrictlyProperBlock stackb = transferFunction(numBlocks, storage);
	struct StrictlyProperBlock * heapb = malloc(sizeof(struct StrictlyProperBlock));
	if (heapb)
		memcpy(heapb, &stackb, sizeof(struct StrictlyProperBlock));
	return heapb;
}

void transferFunction_free(struct StrictlyProperBlock * b)
{
	free(b);
}