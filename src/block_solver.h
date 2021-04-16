#ifndef _BLOCK_SOLVER_H_
#define _BLOCK_SOLVER_H_

#include "block.h"

typedef void(*input_function)(
	FLOAT_TYPE time,
	size_t num_inputs,
	FLOAT_TYPE input[]
	);

typedef void (*solver_function)
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
);

struct rk4_storage
{
	FLOAT_TYPE next_input[], // (1 x numInputs) temp
	FLOAT_TYPE dB[], // (1 x numStates) temp
	FLOAT_TYPE dC[], // (1 x numStates) temp
	FLOAT_TYPE dD[], // (1 x numStates) temp
};

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
);

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
);

// void solve( solver_function solver, struct block const * block, 
#endif
