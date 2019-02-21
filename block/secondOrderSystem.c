#include <dbg.h>
#include "secondOrderSystem.h"

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
	struct secondOrderSystemStorage const * storage = block->storage;

	FLOAT_TYPE const state1[] = state;
	FLOAT_TYPE const state2[] = state + numInputs;
	FLOAT_TYPE const dState1[] = dState;
	FLOAT_TYPE const dState2[] = dState + numInputs;

	memcpy(dState1, state2, numInputs * sizeof(FLOAT_TYPE));

	for (size_t i = 0; i < numInputs; i++)
		dState2[i] = -2 * storage[i].zeta * storage[i].omega_n * state2[i] - storage[i].omega_n * storage[i].omega_n * ( state1[i] - input[i] );
}

FLOAT_TYPE * secondOrderSystemOutput( FLOAT_TYPE output[], size_t numOutputs, FLOAT_TYPE const state[] )
{
	if ( numOutputs != block->numInputs)
		return NULL;
	memcpy(output, state, numOutputs*sizeof(FLOAT_TYPE));
	return output;
}

struct block * secondOrderSystem( struct block * block, size_t numBlocks, struct secondOrderSystemStorage storage[] )
{
	if ( !block || !numBlocks || !storage )
		return NULL;
	block->numStates = numBlocks*2;
	block->numInputs = numBlocks;
	block->storage = storage;
	block->f = physics;
	return block;
}
