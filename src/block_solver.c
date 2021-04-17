#include <string.h>
#include "block_solver.h"

void euler
(
 	void * solver_storage,
	struct block const * block,
	FLOAT_TYPE dt,
	FLOAT_TYPE * time,
	FLOAT_TYPE next_state[], // (1 x numStates)
	FLOAT_TYPE dstate[], // (1 x numStates)
	FLOAT_TYPE output[], // (1 x numOutputs)
	FLOAT_TYPE input[], // (1 x numInputs)
	FLOAT_TYPE const state[], // (1 x numStates)
	input_function inputf
)
{
	size_t i;
	void * storage = block->storage;
	size_t num_states = block->num_states;
	size_t num_inputs = block->num_inputs;
	size_t num_outputs = block->num_outputs;
	physics_function f = block->f;
	(void)solver_storage;

	inputf( *time, num_inputs, input );
	f( *time, num_states, dstate, state, num_inputs, input, num_outputs, output, storage );

	for (i = 0; i < num_states; i++)
		next_state[i] = state[i] + dt * dstate[i];

	*time += dt;
}

void rk4
(
 	void * solver_storage,
	struct block const * block,
	FLOAT_TYPE dt,
	FLOAT_TYPE * time,
	FLOAT_TYPE next_state[], // (1 x numStates)
	FLOAT_TYPE dstate[], // (1 x numStates)
	FLOAT_TYPE output[], // (1 x numOutputs)
	FLOAT_TYPE input[], // (1 x numInputs)
	FLOAT_TYPE const state[], // (1 x numStates)
	input_function inputf
)
{
	size_t i;
	void * storage = block->storage;
	size_t num_states = block->num_states;
	size_t num_inputs = block->num_inputs;
	size_t num_outputs = block->num_outputs;
	physics_function f = block->f;

	struct rk4_storage * rk4_storage = (struct rk4_storage *)solver_storage;
	FLOAT_TYPE * next_input = rk4_storage->next_input;
	FLOAT_TYPE * dB = rk4_storage->dB;
	FLOAT_TYPE * dC = rk4_storage->dC;
	FLOAT_TYPE * dD = rk4_storage->dD;

	FLOAT_TYPE half_dt = dt / 2.0;
	FLOAT_TYPE half_time = *time + half_dt;
	FLOAT_TYPE next_time = *time + dt;

	memcpy(input, next_input, num_inputs);
	f( *time, num_states, dstate, state, num_inputs, next_input, num_outputs, output, storage );
	
	for (i = 0; i < num_states; i++)
		next_state[i] = state[i] + half_dt * dstate[i];
	inputf( half_time, num_inputs, next_input );
	f( half_time, num_states, dB, next_state, num_inputs, next_input, 0, NULL, storage );
	
	for (i = 0; i < num_states; i++)
		next_state[i] = state[i] + half_dt * dB[i];
	f( half_time, num_states, dC, next_state, num_inputs, next_input, 0, NULL, storage );
	
	for (i = 0; i < num_states; i++)
		next_state[i] = state[i] + dt * dC[i];
	inputf( *time, num_inputs, next_input );
	f( next_time, num_states, dD, next_state, num_inputs, next_input, 0, NULL, storage );
	
	for (i = 0; i < num_states; i++)
		next_state[i] = state[i] + dt * (dstate[i] + 2.0*dB[i] + 2.0*dC[i] + dD[i]) / 6.0;

	*time = next_time;
}
