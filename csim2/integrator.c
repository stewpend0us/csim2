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
	memcpy(output, state, numStates * sizeof(double));
}

struct StrictlyProperBlock integrator(size_t const numSignals)
{
	struct StrictlyProperBlock b;
	b.numStates = numSignals;
	b.numInputs = numSignals;
	b.numOutputs = numSignals;
	b.f = integrator_physics;
	b.h = integrator_output;
	b.storage = NULL;
	b.free = NULL;
	return b;
}

struct StrictlyProperBlock * integrator_new(size_t const numSignals)
{
	struct StrictlyProperBlock stackb = integrator(numSignals);
	struct StrictlyProperBlock * heapb = malloc(sizeof(struct StrictlyProperBlock));
	memcpy(heapb, &stackb, sizeof(struct StrictlyProperBlock));
	return heapb;
}

void integrator_free(struct StrictlyProperBlock * b)
{
	free(b);
}