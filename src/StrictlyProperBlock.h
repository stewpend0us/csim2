#pragma once
#include <stdlib.h>
#include "dbg.h"

struct StrictlyProperBlock;

// update dState as a function of time, state, input, and storage
typedef void(*PhysicsFunction)(
	struct StrictlyProperBlock const * const block,
	double * const dState,
	double const time,
	double const * const state,
	double const * const input
	);

// update output as a function of time, state, and storage
typedef void(*OutputFunction)(
	struct StrictlyProperBlock const * const block,
	double * const output,
	double const time,
	double const * const state
	);

typedef void(*UtilityFunction)(
	struct StrictlyProperBlock const * const block,
	double const time,
	double const * const dState,
	double const * const state,
	double const * const input,
	double const * const output
	);

// StrictlyProperBlock data
struct StrictlyProperBlock
{
	size_t numStates;
	size_t numInputs;
	size_t numOutputs;
	void * storage;
	OutputFunction h;
	PhysicsFunction f;
	UtilityFunction u;
};

#define NULL_StritclyProperBlock ((struct StrictlyProperBlock){0,0,0,NULL,NULL,NULL,NULL})
#define good_block(block) (block.h && block.f)