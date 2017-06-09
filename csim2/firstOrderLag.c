#include "firstOrderLag.h"

static void firstOrderLag_physics
(
	size_t numStates,
	size_t numInputs,
	double * const dState,
	double const time,
	double const * const state,
	double const * const input,
	double * const tau
)
{
	for (size_t i = 0; i<numStates; i++)
		dState[i] = (input[i] - state[i]) / tau[i];
}

static void firstOrderLag_output
(
	size_t numStates,
	size_t numOutputs,
	double * const output,
	double const time,
	double const * const state,
	double * const tau
)
{
	memcpy(output, state, numStates * sizeof(double));
}

struct StrictlyProperBlock firstOrderLag(size_t numSignals, double * tau)
{
	struct StrictlyProperBlock b;
	b.numInputs = numSignals;
	b.numOutputs = numSignals;
	b.numStates = numSignals;
	b.storage = tau;
	b.f = firstOrderLag_physics;
	b.h = firstOrderLag_output;
	return b;
}