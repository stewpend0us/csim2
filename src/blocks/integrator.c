#include <string.h>
#include <assert.h>
#include "integrator.h"

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
	(void)state;
	(void)storage;
	memcpy(dState, input, numStates * sizeof(double));
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

struct StrictlyProperBlock integrator(size_t const numBlocks)
{
	assert(numBlocks > 0);
	struct StrictlyProperBlock b;
	b.numStates = numBlocks;
	b.numInputs = numBlocks;
	b.numOutputs = numBlocks;
	b.f = physics;
	b.h = output;
	b.storage = NULL;
	return b;
}
