#pragma once
#include <stdlib.h>

struct StrictlyProperBlockInfo
{
	size_t numStates;
	size_t numInputs;
	size_t numOutputs;
	void * storage;
};

// update dState as a function of time, state, input, and storage
typedef void(*PhysicsFunction)(
	struct StrictlyProperBlockInfo const * const info,
	double * const dState,
	double const time,
	double const * const state,
	double const * const input
	);

// update output as a function of time, state, and storage
typedef void(*OutputFunction)(
	struct StrictlyProperBlockInfo const * const info,
	double * const output,
	double const time,
	double const * const state
	);

// StrictlyProperBlock data
struct StrictlyProperBlock
{
	struct StrictlyProperBlockInfo const info;
	PhysicsFunction const f;
	OutputFunction const h;
};