#include <dbg.h>
#include "firstOrderLag.h"

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
	size_t const numStates = info->numStates;
	double const * const tau = info->storage;

	for (size_t i = 0; i<numStates; i++)
		dState[i] = (input[i] - state[i]) / tau[i];
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
	size_t const numStates = info->numStates;	
	memcpy(output, state, numStates * sizeof(double));
}

struct StrictlyProperBlock firstOrderLag(size_t const numBlocks, double * const tau)
{
	check(numBlocks > 0,"numBlocks must be greater than zero");
	check(tau, "tau must not be NULL");
	for (size_t i = 0; i < numBlocks; i++)
	{
		check(tau[i] > 0, "tau[%zu] must be greater than zero",i);
	}
	return (struct StrictlyProperBlock)
	{
		{
			numBlocks,
			numBlocks,
			numBlocks,
			tau,
		},
		output,
		physics,
		NULL,
	};
error:
	return NULL_StritclyProperBlock;
}