#include <dbg.h>
#include "firstOrderLag.h"

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
	size_t const numStates = block->numStates;
	double const * const tau = block->storage;

	for (size_t i = 0; i<numStates; i++)
		dState[i] = (input[i] - state[i]) / tau[i];
}

struct StrictlyProperBlock firstOrderLag(size_t const numBlocks, double * const tau, UtilityFunction const util)
{
	check(numBlocks > 0,"numBlocks must be greater than zero");
	check(tau, "tau must not be NULL");
	for (size_t i = 0; i < numBlocks; i++)
	{
		check(tau[i] > 0, "tau[%zu] must be greater than zero",i);
	}
	return (struct StrictlyProperBlock)
	{
		numBlocks,
		numBlocks,
		numBlocks,
		tau,
		outputState,
		physics,
		util,
	};
error:
	return NULL_StritclyProperBlock;
}