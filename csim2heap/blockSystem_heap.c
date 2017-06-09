#include <assert.h>
#include <string.h>
#include "blockSystem_heap.h"
#include "../csim2/blockSystem.h"

struct StrictlyProperBlock * blockSystem_new
(
	size_t const numSystemInputs,
	size_t const numSystemOutputs,
	struct BlockSystemStorage * const storage
)
{
	struct StrictlyProperBlock stackb = blockSystem(numSystemInputs, numSystemOutputs, storage);
	struct StrictlyProperBlock * heapb = malloc(sizeof(struct StrictlyProperBlock));
	memcpy(heapb, &stackb, sizeof(struct StrictlyProperBlock));
	return heapb;
}

void blockSystem_free(struct StrictlyProperBlock * b)
{
	free(b);
}