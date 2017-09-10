#include <string.h>
#include <assert.h>
#include "firstOrderLag.h"

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
	(void)numInputs;
	(void)time;
	double const * const tau = storage;

	for (size_t i = 0; i<numStates; i++)
		dState[i] = (input[i] - state[i]) / tau[i];
}

static void output
(
	size_t const numStates,
	size_t const numOutputs,
	double * const output,
	double const time,
	double const * const state,
	void * const storage
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