#include "block_basic.h"
#include <string.h>

static void integrator_physics
(
	FLOAT_TYPE time,
	size_t num_states,
	FLOAT_TYPE dstate[],
	FLOAT_TYPE const state[],
	size_t num_inputs,
	FLOAT_TYPE const input[],
	size_t num_outputs,
	FLOAT_TYPE output[],
	void * storage
)
{
	(void)time;
	(void)state;
	(void)num_inputs;
	(void)num_outputs;
	(void)output;
	(void)storage;
	memcpy( dstate, input, num_states * sizeof(FLOAT_TYPE) );
}

struct block * integrator( struct block * block, size_t num_blocks )
{
	if ( !block || !num_blocks ) return NULL;

	block->num_states = num_blocks;
	block->num_inputs = num_blocks;
	block->num_outputs = 0;
	block->storage = NULL;
	block->f = integrator_physics;
	return block;
}

static void first_order_lag_physics
(
	FLOAT_TYPE time,
	size_t num_states,
	FLOAT_TYPE dstate[],
	FLOAT_TYPE const state[],
	size_t num_inputs,
	FLOAT_TYPE const input[],
	size_t num_outputs,
	FLOAT_TYPE output[],
	void * storage
)
{
	(void)time;
	(void)num_inputs;
	(void)num_outputs;
	(void)output;
	FLOAT_TYPE const * tau = storage;

	for ( size_t i = 0; i < num_states; i++ )
		dstate[i] = (input[i] - state[i]) / tau[i];
}

struct block * first_order_lag( struct block * block, size_t num_blocks, FLOAT_TYPE tau[] )
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
	block->num_outputs = 0;
	block->storage = tau;
	block->f = first_order_lag_physics;
	return block;
}

// note that the typical "output" of this block would be the first half of the state vector
static void second_order_system_physics
(
	FLOAT_TYPE time,
	size_t num_states,
	FLOAT_TYPE dstate[],
	FLOAT_TYPE const state[],
	size_t num_inputs,
	FLOAT_TYPE const input[],
	size_t num_outputs,
	FLOAT_TYPE output[],
	void * storage
)
{
	(void)time;
	(void)num_states;
	(void)num_outputs;
	(void)output;
	struct second_order_system_storage * so_storage = storage;

	FLOAT_TYPE const * state1 = state;
	FLOAT_TYPE const * state2 = state + num_inputs;
	FLOAT_TYPE * dstate1 = dstate;
	FLOAT_TYPE * dstate2 = dstate + num_inputs;

	memcpy(dstate1, state2, num_inputs * sizeof(FLOAT_TYPE));

	for ( size_t i = 0; i < num_inputs; i++ )
	{
		FLOAT_TYPE zeta = so_storage[i].zeta;
		FLOAT_TYPE omega_n = so_storage[i].omega_n;
		FLOAT_TYPE numerator = so_storage[i].numerator;
		dstate2[i] = -2.0 * zeta * omega_n * state2[i] - omega_n * omega_n * state1[i] + numerator * input[i];
	}
}

struct block * second_order_system( struct block * block, size_t num_blocks, struct second_order_system_storage storage[] )
{
	if ( !block || !num_blocks || !storage )
		return NULL;
	block->num_states = num_blocks*2;
	block->num_inputs = num_blocks;
	block->num_outputs = 0;
	block->storage = storage;
	block->f = second_order_system_physics;
	return block;
}
/*
static void transfer_function_physics
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
	struct transferFunctionStorage * tf_storage = block->storage;
	size_t xi = 0;
	size_t n_states;
	for (size_t i = 0; i < num_inputs; i++)
	{
		n_states = tf_storage[i].den_size - 1;
		memcpy(&dState[xi], &state[xi + 1], n_states - 1); //dx1 = x2, dx2 = x3, ...
		
		dState[n_states - 1] = input[i];
		for (size_t j = 0; j < n_states; j++)
			dState[n_states - 1] -= tf_storage[i].denominator[n_states - j] * state[xi + j];
		dState[n_states - 1] /= tf_storage[i].denominator[0];
		xi += n_states;
	}
}

static void output
(
	struct StrictlyProperBlock const * const block,
	double * const output,
	double const time,
	double const * const state
)
{
	(void)time;
	size_t const numOutputs = block->numOutputs;
	struct transferFunctionStorage const * const tf_storage = block->storage;

	size_t xi = 0;
	for (size_t i = 0; i < numOutputs; i++)
	{
		output[i] = 0;
		for (size_t j = 0; j < tf_storage[i].num_size; j++)
			output[i] = tf_storage[i].numerator[tf_storage[i].num_size - 1 - j] * state[xi + j];
	}
}

struct StrictlyProperBlock transferFunction(
	size_t const numBlocks,
	struct transferFunctionStorage * const tf_storage,
	UtilityFunction const util
)
{
	check(numBlocks > 0, "numBlocks must be greater than zero");
	check(tf_storage, "tf_storage must not be NULL");

	size_t totalStates = 0;
	for (size_t i = 0; i < numBlocks; i++)
	{
		check(tf_storage[i].num_size > 0, "tf_storage[%zu].num_size must be greater than zero", i);
		check(tf_storage[i].den_size > tf_storage[i].num_size, "tf_storage[%zu].den_size must be greater than tf_storage[%zu].num_size",i,i); // requirement of "strictly proper" systems
		check(tf_storage[i].denominator[0] != 0.0, "tf_storage[%zu].denominator[0] must not be zero", i);
		totalStates += tf_storage[i].den_size - 1;
	}
	return (struct StrictlyProperBlock)
	{
		totalStates,
		numBlocks,
		numBlocks,
		tf_storage,
		output,
		physics,
		util,
	};
error:
	return NULL_StritclyProperBlock;
}
*/
