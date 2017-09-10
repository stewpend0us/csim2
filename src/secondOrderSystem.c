#include <string.h>
#include <assert.h>
#include "secondOrderSystem.h"

static void physics
(
	size_t const numStates,
	size_t const numInputs,
	double * const dState,
	double const time,
	double const * const state,
	double const * const input,
	void * const storage
)
{
	(void)numStates;
	(void)time;
	struct secondOrderSystemStorage const * const so_storage = storage;

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
	size_t const numStates,
	size_t const numOutputs,
	double * const output,
	double const time,
	double const * const state,
	void * storage
)
{
	(void)numStates;
	(void)time;
	(void)storage;

	double const * const state1 = state;
	memcpy(output, state1, numOutputs * sizeof(double));
}

struct StrictlyProperBlock secondOrderSystem(size_t const numBlocks, struct secondOrderSystemStorage * const so_storage)
{
	assert(numBlocks > 0);
	for (size_t i = 0; i < numBlocks; i++)
	{
		assert(so_storage[i].omega_n > 0);
		assert(so_storage[i].zeta > 0);
	}

	struct StrictlyProperBlock b;
	b.numInputs = numBlocks;
	b.numOutputs = numBlocks;
	b.numStates = 2 * numBlocks;
	b.storage = so_storage;
	b.f = physics;
	b.h = output;
	return b;
}