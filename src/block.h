#ifndef _BLOCK_H_
#define _BLOCK_H_

#include <stdlib.h>

struct block;

typedef void(*physics_function)(
	FLOAT_TYPE time,
	size_t num_states,
	FLOAT_TYPE dstate[],
	FLOAT_TYPE const state[],
	size_t num_inputs,
	FLOAT_TYPE const input[],
	size_t num_outputs,
	FLOAT_TYPE output[],
	void * storage
	);

struct block
{
	size_t num_states; // number of states 
	size_t num_inputs; // number of inputs
	size_t num_outputs; // number of outputs
	void * storage; // point to anything you want here
	physics_function f; // dState = f(time, state, input)
};

#endif
