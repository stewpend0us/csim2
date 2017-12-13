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

static void output
(
	struct StrictlyProperBlock const * const block,
	double * const output,
	double const time,
	double const * const state
)
{
	(void)time;
	memcpy(output, state, block->numStates * sizeof(double));
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
		output,
		physics,
		util,
	};
error:
	return NULL_StritclyProperBlock;
}
