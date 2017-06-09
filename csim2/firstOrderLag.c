#include <string.h>
#include "firstOrderLag.h"

static void physics
(
	size_t const numStates,
	size_t const numInputs,
	double * const dState,
	double const time,
	double const * const state,
	double const * const input,
	double const * const tau
)
{
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
	double const * const tau
)
{
	memcpy(output, state, numStates * sizeof(double));
}

struct StrictlyProperBlock firstOrderLag(size_t const numBlocks, double * const tau)
{
	struct StrictlyProperBlock b;
	b.numInputs = numBlocks;
	b.numOutputs = numBlocks;
	b.numStates = numBlocks;
	b.storage = tau;
	b.f = physics;
	b.h = output;
	return b;
}