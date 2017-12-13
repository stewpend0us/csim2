#include <dbg.h>
#include "secondOrderSystem.h"

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
	size_t const numInputs = block->numInputs;

	struct secondOrderSystemStorage const * const so_storage = block->storage;

	double const * const state1 = state;
	double const * const state2 = state + numInputs;
	double * const dState1 = dState;
	double * const dState2 = dState + numInputs;

	memcpy(dState1, state2, numInputs * sizeof(double));

	for (size_t i = 0; i < numInputs; i++)
		dState2[i] = -2 * so_storage[i].zeta * so_storage[i].omega_n * state2[i] - so_storage[i].omega_n * so_storage[i].omega_n * ( state1[i] - input[i] );
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
	size_t const numOutputs = block->numOutputs;
	double const * const state1 = state; // just a reminder that there are 2 groups of state (2nd order)
	memcpy(output, state1, numOutputs * sizeof(double));
}

struct StrictlyProperBlock secondOrderSystem(
	size_t const numBlocks,
	struct secondOrderSystemStorage * const so_storage,
	UtilityFunction const util
)
{
	check(numBlocks > 0,"numBlocks must be greater than zero");
	check(so_storage, "so_storage must not be NULL");
	for (size_t i = 0; i < numBlocks; i++)
	{
		check(so_storage[i].omega_n > 0,"so_storage[%zu].omega_n must be greater than zero",i);
		check(so_storage[i].zeta > 0,"so_storage[%zu].zeta must be greater than zero",i);
	}
	return (struct StrictlyProperBlock)
	{
		2*numBlocks,
		numBlocks,
		numBlocks,
		so_storage,
		output,
		physics,
		util,
	};
error:
	return NULL_StritclyProperBlock;
}