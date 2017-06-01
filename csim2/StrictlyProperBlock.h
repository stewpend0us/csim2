#pragma once
#include <stdlib.h>

// update dState as a function of time, state, and input
typedef void(*PhysicsFunction)(
	size_t const numDstates,
	double * const dState,
	double const time,
	size_t const numStates,
	double const * const state,
	size_t const numInputs,
	double const * const input,
	void * const storage
	);

// update output as a function of time, and state
typedef void(*OutputFunction)(
	size_t const numOutputs,
	double * const output,
	double const time,
	size_t const numStates,
	double const * const state,
	void * const storage
	);

// constructor function to be exported by dlls
typedef struct StrictlyProperBlock(*StrictlyProperBlockDLLConstructor)();

// StrictlyProperBlock data
struct StrictlyProperBlock
{
	size_t const numStates;
	size_t const numInputs;
	size_t const numOutputs;
	PhysicsFunction const f;
	OutputFunction const h;
	void * const storage;
};