#include "integrator.h"
#include <string.h>

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
	(void)state;
	memcpy( dState, input, block->numStates * sizeof(FLOAT_TYPE) );
}

struct block * integrator( struct block * block, size_t numBlocks )
{
	if ( !block || !numBlocks ) return NULL;

	block->numStates = numBlocks;
	block->numInputs = numBlocks;
	block->storage = NULL;
	block->f = physics;
	return block;
}
