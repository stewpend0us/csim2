#ifndef _BLOCK_H_
#define _BLOCK_H_

#include <stdlib.h>

struct block;

typedef void(*dStateFunction)(
	struct block const * block,
	FLOAT_TYPE dState[],
	FLOAT_TYPE time,
	FLOAT_TYPE const state[],
	FLOAT_TYPE const input[]
	);

struct block
{
	size_t numStates; // number of states 
	size_t numInputs; // number of inputs
	void * storage; // point to anything you want here
	dStateFunction f; // dState = f(time, state, input)
};

#endif
