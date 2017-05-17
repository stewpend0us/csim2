#include <assert.h>
#include <string.h>
#include "StrictlyProperBlock.h"

static void integrator_f(double * const dState, double t, double const * const state, double const * const input, size_t const numStates, size_t const numInputs, void * const storage)
{
	assert(numStates == numInputs);
	memcpy(dState, input, numStates * sizeof(double));
}

static void integrator_h(double * const output, double t, double const * const state, size_t numStates, size_t const numOutputs, void * const storage)
{
	assert(numStates == numOutputs);
	memcpy(output, state, numStates * sizeof(double));
}

struct StrictlyProperBlock integrator(size_t numSignals)
{
	return (struct StrictlyProperBlock)
	{
		.numStates = numSignals, .numInputs = numSignals, .numOutputs = numSignals, .f = integrator_f, .h = integrator_h, .storage = NULL,
	};
}

struct StrictlyProperBlock system_new(size_t numInputs, size_t numOutputs, struct StrictlyProperBlock * blocks, void(*updateInputs)(double ** inputs, double const ** const outputs))
{
	return (struct StrictlyProperBlock) { .numInputs = numInputs, .numOutputs = numOutputs };
}

void system_free(struct StrictlyProperBlock * system)
{

}