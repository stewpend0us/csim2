#include <dbg.h>
#include "integrator.h"

static void physics
(
	struct StrictlyProperBlock const * const block,
	double * const dState,
	double const time,
	double const * const state,
	double const * const input
)
{
	(void)time;
	(void)state;
	memcpy(dState, input, block->numStates * sizeof(double));
}

struct StrictlyProperBlock integrator(size_t const numBlocks, UtilityFunction const util)
{
	check(numBlocks > 0, "numBlocks must be greater than 0");
	return (struct StrictlyProperBlock)
	{
		numBlocks,
		numBlocks,
		numBlocks,
		NULL,
		outputState,
		physics,
		util,
	};
error:
	return NULL_StritclyProperBlock;
}
