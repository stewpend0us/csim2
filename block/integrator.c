#include "integrator.h"
#include <string.h>

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
	(void)state;
	(void)num_inputs;
	(void)storage;
	memcpy( dstate, input, num_states * sizeof(FLOAT_TYPE) );
}

struct block * integrator( struct block * block, size_t num_blocks )
{
	if ( !block || !num_blocks ) return NULL;

	block->num_states = num_blocks;
	block->num_inputs = num_blocks;
	block->storage = NULL;
	block->f = physics;
	return block;
}
