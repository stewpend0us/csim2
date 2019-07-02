#include "solver.h"

void euler
(
	struct block const * block,
	FLOAT_TYPE next_state[],
	FLOAT_TYPE dstate[],
	FLOAT_TYPE dt,
	FLOAT_TYPE * time,
	FLOAT_TYPE const state[],
	FLOAT_TYPE const input[]
)
{
	size_t i;
	size_t num_states = block->num_states;

	block->f( *time, num_states, dstate, state, block->num_inputs, input, block->storage );
	for (i = 0; i < num_states; i++)
		next_state[i] = state[i] + dt * dstate[i];

	*time += dt;
}

void rk4
(
	struct block const * block,
	FLOAT_TYPE next_state[],
	FLOAT_TYPE dstate[],
	FLOAT_TYPE dB[],
	FLOAT_TYPE dC[],
	FLOAT_TYPE dD[],
	FLOAT_TYPE dt,
	FLOAT_TYPE * time,
	FLOAT_TYPE const state[],
	FLOAT_TYPE const input[],
	FLOAT_TYPE const half_step_input[],
	FLOAT_TYPE const next_input[]
)
{
	size_t i;
	size_t num_states = block->num_states;
	size_t num_inputs = block->num_inputs;
	void * storage = block->storage;
	physicsFunction f = block->f;
	FLOAT_TYPE half_dt = dt / 2;
	FLOAT_TYPE half_time = *time + half_dt;
	FLOAT_TYPE next_time = *time + dt;

	f( *time, num_states, dstate, state, num_inputs, input, storage );
	
	for (i = 0; i < num_states; i++)
		next_state[i] = state[i] + half_dt * dstate[i];
	f( half_time, num_states, dB, next_state, num_inputs, half_step_input, storage );
	
	for (i = 0; i < num_states; i++)
		next_state[i] = state[i] + half_dt * dB[i];
	f( half_time, num_states, dC, next_state, num_inputs, half_step_input, storage );
	
	for (i = 0; i < num_states; i++)
		next_state[i] = state[i] + dt * dC[i];
	f( next_time, num_states, dD, next_state, num_inputs, next_input, storage );
	
	for (i = 0; i < num_states; i++)
		next_state[i] = state[i] + dt * (dstate[i] + 2.0*dB[i] + 2.0*dC[i] + dD[i]) / 6.0;

	*time = next_time;
}
