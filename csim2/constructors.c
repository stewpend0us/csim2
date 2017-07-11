#include <string.h>
#include "constructors.h"

struct StrictlyProperBlock * block_new(struct StrictlyProperBlock const stackb)
{
	struct StrictlyProperBlock * heapb = malloc(sizeof(struct StrictlyProperBlock));
	if (heapb)
		memcpy(heapb, &stackb, sizeof(struct StrictlyProperBlock));
	return heapb;
}

void block_free(struct StrictlyProperBlock * heapb)
{
	free(heapb);
}