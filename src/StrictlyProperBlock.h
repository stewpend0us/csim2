#pragma once
#include <stdlib.h>
#include "dbg.h"

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

typedef void(*UtilityFunction)(
	struct StrictlyProperBlock const * const info,
	double const time,
	double const * const dState,
	double const * const state,
	double const * const input,
	double const * const output
);

// StrictlyProperBlock data
struct StrictlyProperBlock
{
	struct StrictlyProperBlockInfo info;
	OutputFunction h;
	PhysicsFunction f;
	UtilityFunction u;
};

#define NULL_StritclyProperBlock ((struct StrictlyProperBlock){{0,0,0,NULL},NULL,NULL,NULL})
#define good_block(block) (block.h && block.f)