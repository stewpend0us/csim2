#pragma once
#include <stdlib.h>

// update dState as a function of time, state, and input
typedef void(*PhysicsFunction)(
	size_t numStates,
	size_t numInputs,
	double * const dState,
	double const time,
	double const * const state,
	double const * const input,
	void * const storage
	);

// update output as a function of time, and state
typedef void(*OutputFunction)(
	size_t numStates,
	size_t numOutputs,
	double * const output,
	double const time,
	double const * const state,
	void * const storage
	);

// StrictlyProperBlock data
struct StrictlyProperBlock
{
	size_t numStates;
	size_t numInputs;
	size_t numOutputs;
	PhysicsFunction f;
	OutputFunction h;
	void * storage;
};

// constructor function to be exported by dlls
typedef struct StrictlyProperBlock(*StrictlyProperBlockDLLConstructor)();