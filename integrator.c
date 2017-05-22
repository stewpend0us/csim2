#include <assert.h>
#include <string.h>
#include "integrator.h"

static void integrator_physics
(
	size_t const numDstates,
	double * const dState,
	double const time,
	size_t const numStates,
	double const * const state,
	size_t const numInputs,
	double const * const input,
	void * const storage
)
{
	assert(numStates == numInputs);
	memcpy(dState, input, numStates * sizeof(double));
}

static void integrator_output
(
	size_t const numOutputs,
	double * const output,
	double const time,
	size_t const numStates,
	double const * const state,
	void * const storage
)
{
	assert(numStates == numOutputs);
	memcpy(output, state, numStates * sizeof(double));
}

struct StrictlyProperBlock integrator(size_t const numSignals)
{
	return (struct StrictlyProperBlock)
	{
		.numStates = numSignals, .numInputs = numSignals, .numOutputs = numSignals, .f = integrator_physics, .h = integrator_output, .storage = NULL,
	};
}