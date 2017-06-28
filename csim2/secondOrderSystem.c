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
	struct secondOrderSystemStorage const * const storage
)
{
	double const * const state1 = state;
	double const * const state2 = state + numInputs;
	double * const dState1 = dState;
	double * const dState2 = dState + numInputs;

	memcpy(dState1, state2, numInputs * sizeof(double));

	for (size_t i = 0; i < numInputs; i++)
		dState2[i] = -2 * storage[i].zeta * storage[i].omega_n * state2[i] - storage[i].omega_n * storage[i].omega_n * ( state1[i] - input[i] );
}

static void output
(
	size_t const numStates,
	size_t const numOutputs,
	double * const output,
	double const time,
	double const * const state,
	struct secondOrderSystemStorage const * const storage
)
{
	double const * const state1 = state;
	memcpy(output, state1, numOutputs * sizeof(double));
}

struct StrictlyProperBlock secondOrderSystem(size_t const numBlocks, struct secondOrderSystemStorage * const storage)
{
	assert(numBlocks > 0);
	for (size_t i = 0; i < numBlocks; i++)
	{
		assert(storage[i].omega_n > 0);
		assert(storage[i].zeta > 0);
	}

	struct StrictlyProperBlock b;
	b.numInputs = numBlocks;
	b.numOutputs = numBlocks;
	b.numStates = 2 * numBlocks;
	b.storage = storage;
	b.f = physics;
	b.h = output;
	return b;
}