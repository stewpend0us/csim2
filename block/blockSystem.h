#ifndef _BLOCKSYSTEM_H_
#define _BLOCKSYSTEM_H_

#include "block.h"

typedef void(*updateChildInputFunction)(
	struct blockSystem const * blockSystem,
	FLOAT_TYPE time,
	FLOAT_TYPE * childState[], // array of child block state arrays
	FLOAT_TYPE const systemInput[],
	);

struct blockSystem
{
	size_t numChildren; // number of child blocks
	size_t numInputs; // number of inputs
	struct block const child[]; // array of child blocks
	FLOAT_TYPE * childInput[]; // array of child block inputs
	void * storage; // put anything you want here
	updateChildInputFunction calcChildInput; // childInput = f(time, childStates, systemInput)
};


struct block * blockSystem
(
	struct blockSystem * system
);

#endif
