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

// StrictlyProperBlock data
struct StrictlyProperBlock
{
	struct StrictlyProperBlockInfo info;
	PhysicsFunction f;
	OutputFunction h;
};

#define NULL_StritclyProperBlock ((struct StrictlyProperBlock){{0,0,0,NULL},NULL,NULL})
#define good_block(block) (block.f && block.h)