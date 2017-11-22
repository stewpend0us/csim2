#include <string.h>
#include <assert.h>
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
	(void)numInputs;
	(void)time;
	double const * const tau = storage;

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
	(void)numOutputs;
	(void)time;
	(void)storage;
	
	memcpy(output, state, numStates * sizeof(double));
}

struct StrictlyProperBlock firstOrderLag(size_t const numBlocks, double * const tau)
{
	assert(numBlocks > 0);
	for (size_t i = 0; i < numBlocks; i++)
	{
		assert(tau[i] > 0);
	}

	struct StrictlyProperBlock b;
	b.numInputs = numBlocks;
	b.numOutputs = numBlocks;
	b.numStates = numBlocks;
	b.storage = tau;
	b.f = physics;
	b.h = output;
	return b;
}