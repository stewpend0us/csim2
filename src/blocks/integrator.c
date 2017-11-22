#include <dbg.h>
#include "integrator.h"

static void physics
(
	struct StrictlyProperBlockInfo const * const info,
	double * const dState,
	double const time,
	double const * const state,
	double const * const input
)
{
	(void)time;
	(void)state;
	memcpy(dState, input, info->numStates * sizeof(double));
}

static void output
(
	struct StrictlyProperBlockInfo const * const info,
	double * const output,
	double const time,
	double const * const state
)
{
	(void)time;
	memcpy(output, state, info->numStates * sizeof(double));
}

struct StrictlyProperBlock integrator(size_t const numBlocks)
{
	
	check(numBlocks > 0, "numBlocks must be greater than 0");
	return (struct StrictlyProperBlock)
	{
		{
			numBlocks,
			numBlocks,
			numBlocks,
			NULL,
		},
		physics,
		output,
	};
error:
	return NULL_StritclyProperBlock;
}
