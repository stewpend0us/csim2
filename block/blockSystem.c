#include "blockSystem.h"

static void physics
(
	struct block const * block,
	FLOAT_TYPE dState[],
	FLOAT_TYPE time,
	FLOAT_TYPE const state[],
	FLOAT_TYPE const input[]
)
{
	struct blockSystem * system = block->storage;
	size_t numChildren = system->numChildren;
	struct block * child = system->child;
	FLOAT_TYPE ** childInput = system->childInput;
	FLOAT_TYPE const ** childState = system->childState;
	size_t i, xi;
	for (i = 0, xi = 0; i < numChildren; xi += child[i++].numStates)
		childState[i] = &state[xi];
	system->updateChildInput( system, time, childState, input );
	for (i = 0, xi = 0; i < numChildren; xi += child[i++].numStates)
		child[i].f(&child[i], &dState[xi], time, &state[xi], childInput[i]);
}

struct block * blockSystem( struct block * block, struct blockSystem * system )
{
	if ( !block || !system || !system->numChildren || !system->child || !system->childInput || !system->childState || !system->updateChildInput )
		return NULL;

	size_t numChildren = system->numChildren;
	struct block * child = system->child;
	FLOAT_TYPE ** childInput = system->childInput;

	size_t numStates = 0;
	for (size_t i = 0; i < numChildren; i++)
	{
		if ( child[i].numStates == 0 || childInput[i] == NULL )
			return NULL;
		numStates += child[i].numStates;
	}
	block->numStates = numStates;
	block->numInputs = system->numInputs;
	block->storage = system;
	block->f = physics;
	return block;
}
