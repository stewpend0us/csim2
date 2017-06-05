#pragma once
#include <stdlib.h>

// update dState as a function of time, state, input, and storage
typedef void(*PhysicsFunction)(
	size_t numStates,
	size_t numInputs,
	double * const dState,
	double const time,
	double const * const state,
	double const * const input,
	void * const storage
	);

// update output as a function of time, state, and storage
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