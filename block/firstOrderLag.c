#include "firstOrderLag.h"

static void physics
(
	struct block const * block,
	FLOAT_TYPE dState[],
	FLOAT_TYPE time,
	FLOAT_TYPE const state[],
	FLOAT_TYPE const input[]
)
{
	(void)time;
	size_t numStates = block->numStates;
	FLOAT_TYPE const * tau = block->storage;

	for (size_t i = 0; i<numStates; i++)
		dState[i] = (input[i] - state[i]) / tau[i];
}

struct block * firstOrderLag( struct block * block, size_t numBlocks, FLOAT_TYPE * tau )
{
	if ( !block || !numBlocks || !tau ) return NULL;
	for (size_t i = 0; i < numBlocks; i++)
	{
		if (tau[i] <= 0)
			return NULL;
	}
	block->numStates = numBlocks;
	block->numInputs = numBlocks;
	block->storage = tau;
	block->f = physics;
	return block;
}
