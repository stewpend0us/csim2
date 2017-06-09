#include <string.h>
#include "integrator_heap.h"

struct StrictlyProperBlock * integrator_new(size_t const numSignals)
{
	struct StrictlyProperBlock stackb = integrator(numSignals);
	struct StrictlyProperBlock * heapb = malloc(sizeof(struct StrictlyProperBlock));
	memcpy(heapb, &stackb, sizeof(struct StrictlyProperBlock));
	return heapb;
}

void integrator_free(struct StrictlyProperBlock * b)
{
	free(b);
}