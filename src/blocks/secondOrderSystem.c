#include <dbg.h>
#include "secondOrderSystem.h"

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
	size_t const numInputs = info->numInputs;

	struct secondOrderSystemStorage const * const so_storage = info->storage;

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
	struct StrictlyProperBlockInfo const * const info,
	double * const output,
	double const time,
	double const * const state
)
{
	(void)time;
	size_t const numOutputs = info->numOutputs;
	double const * const state1 = state; // just a reminder that there are 2 groups of state (2nd order)
	memcpy(output, state1, numOutputs * sizeof(double));
}

struct StrictlyProperBlock secondOrderSystem(size_t const numBlocks, struct secondOrderSystemStorage * const so_storage)
{
	check(numBlocks > 0,"numBlocks must be greater than zero");
	for (size_t i = 0; i < numBlocks; i++)
	{
		check(so_storage[i].omega_n > 0,"so_storage[%zu].omega_n must be greater than zero",i);
		check(so_storage[i].zeta > 0,"so_storage[%zu].zeta must be greater than zero",i);
	}
	return (struct StrictlyProperBlock)
	{
		{
			2*numBlocks,
			numBlocks,
			numBlocks,
			so_storage,
		},
		physics,
		output,
	};
error:
	return NULL_StritclyProperBlock;
}