#include "firstOrderLag.h"

static void physics
(
	FLOAT_TYPE time,
	size_t num_states,
	FLOAT_TYPE dstate[],
	FLOAT_TYPE const state[],
	size_t num_inputs,
	FLOAT_TYPE input[],
	void * storage
)
{
	(void)time;
	(void)num_inputs;
	FLOAT_TYPE const * tau = storage;

	for ( size_t i = 0; i < num_states; i++ )
		dstate[i] = (input[i] - state[i]) / tau[i];
}

struct block * firstOrderLag( struct block * block, size_t num_blocks, FLOAT_TYPE tau[] )
{
	if ( !block || !num_blocks || !tau )
		return NULL;
	for (size_t i = 0; i < num_blocks; i++)
	{
		if (tau[i] == 0.0)
			return NULL;
	}
	block->num_states = num_blocks;
	block->num_inputs = num_blocks;
	block->storage = tau;
	block->f = physics;
	return block;
}
