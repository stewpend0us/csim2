#include <string.h>
#include "constructors.h"

static struct StrictlyProperBlock * _new(struct StrictlyProperBlock const stackb)
{
	struct StrictlyProperBlock * heapb = malloc(sizeof(struct StrictlyProperBlock));
	if (heapb)
		memcpy(heapb, &stackb, sizeof(struct StrictlyProperBlock));
	return heapb;
}

struct StrictlyProperBlock * integrator_new(size_t const numBlocks)
{
	struct StrictlyProperBlock stackb = integrator(numBlocks);
	return _new(stackb);
}
void integrator_free(struct StrictlyProperBlock * b)
{
	free(b);
}

struct StrictlyProperBlock * firstOrderLag_new(size_t const numBlocks, double * const tau)
{
	struct StrictlyProperBlock stackb = firstOrderLag(numBlocks, tau);
	return _new(stackb);
}
void firstOrderLag_free(struct StrictlyProperBlock * b)
{
	free(b);
}

struct StrictlyProperBlock * secondOrderSystem_new(size_t const numBlocks, struct secondOrderSystemStorage * const storage)
{
	struct StrictlyProperBlock stackb = secondOrderSystem(numBlocks, storage);
	return _new(stackb);
}
void secondOrderSystem_free(struct StrictlyProperBlock * b)
{
	free(b);
}

struct StrictlyProperBlock * transferFunction_new(size_t const numBlocks, struct transferFunctionStorage * const storage)
{
	struct StrictlyProperBlock stackb = transferFunction(numBlocks, storage);
	return _new(stackb);
}
void transferFunction_free(struct StrictlyProperBlock * b)
{
	free(b);
}

struct StrictlyProperBlock * blockSystem_new
(
	size_t const numSystemInputs,
	size_t const numSystemOutputs,
	struct BlockSystemStorage * const storage
)
{
	struct StrictlyProperBlock stackb = blockSystem(numSystemInputs, numSystemOutputs, storage);
	return _new(stackb);
}
void blockSystem_free(struct StrictlyProperBlock * b)
{
	free(b);
}