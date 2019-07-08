#include <dbg.h>
#include "secondOrderSystem.h"

// note that the typical "output" of this block would be the first half of the state vector
static void physics
(
	FLOAT_TYPE time,
	size_t num_states,
	FLOAT_TYPE dState[],
	FLOAT_TYPE const state[],
	size_t num_inputs,
	FLOAT_TYPE input[],
	void * storage
)
{
	(void)time;
	(void)num_states;
	struct secondOrderSystemStorage * so_storage = storage;

	FLOAT_TYPE const state1[] = state;
	FLOAT_TYPE const state2[] = state + num_inputs;
	FLOAT_TYPE dState1[] = dState;
	FLOAT_TYPE dState2[] = dState + num_inputs;

	memcpy(dState1, state2, num_inputs * sizeof(FLOAT_TYPE));

	for ( size_t i = 0; i < num_inputs; i++ )
	{
		FLOAT_TYPE zeta = so_storage[i].zeta;
		FLOAT_TYPE omega_n = so_storage[i].omega_n;
		FLOAT_TYPE numerator = so_storage[i].numerator;
		dState2[i] = -2 * zeta * omega_n * state2[i] - omega_n * omega_n * ( state1[i] - input[i] );
	}
}

struct block * secondOrderSystem( struct block * block, size_t num_blocks, struct secondOrderSystemStorage storage[] )
{
	if ( !block || !num_blocks || !storage )
		return NULL;
	block->num_states = num_blocks*2.0;
	block->num_inputs = num_blocks;
	block->storage = storage;
	block->f = physics;
	return block;
}
