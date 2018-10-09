#ifndef _BLOCK_H_
#define _BLOCK_H_

#include <stdlib.h>

struct block
{
	size_t numStates; // number of states 
	size_t numInputs; // number of inputs
	void * storage; // point to anything you want here
	dStateFunction f; // dState = f(time, state, input)
};

typedef void(*dStateFunction)(
	struct block const * block,
	double dState[],
	double time,
	double const state[],
	double const input[]
	);


#endif
