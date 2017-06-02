#include <assert.h>
#include <string.h>
#include "integrator.h"

static void integrator_physics
(
	size_t numStates,
	size_t numInputs,
	double * const dState,
	double const time,
	double const * const state,
	double const * const input,
	void * const storage
)
{
	assert(numStates == numInputs);
	memcpy(dState, input, numStates * sizeof(double));
}

static void integrator_output
(
	size_t numStates,
	size_t numOutputs,
	double * const output,
	double const time,
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