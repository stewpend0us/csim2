#pragma once
#include <stdlib.h>

struct BlockInfo
{
	size_t numStates;
	size_t numInputs;
	size_t numOutputs;
	void * storage;
}

// update dState as a function of time, state, input, and storage
typedef void(*PhysicsFunction)(
	struct BlockInfo const * const info,
	double * const dState,
	double const time,
	double const * const state,
	double const * const input
	);

// update output as a function of time, state, and storage
typedef void(*OutputFunction)(
	struct BlockInfo const * const info,
	double * const output,
	double const time,
	double const * const state,
	void * const storage
	);

// StrictlyProperBlock data
struct StrictlyProperBlock
{
	struct BlockInfo const info;
	PhysicsFunction f;
	OutputFunction h;
};