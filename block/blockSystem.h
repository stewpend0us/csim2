#ifndef _BLOCKSYSTEM_H_
#define _BLOCKSYSTEM_H_

#include "block.h"

struct blockSystem;

typedef void(*updateChildInputsFunction)(
	struct blockSystem const * system,
	FLOAT_TYPE time,
	FLOAT_TYPE const systemState[],
	FLOAT_TYPE const systemInput[]
	);

struct blockSystem
{
	size_t numChildren; // number of child blocks
	size_t numInputs; // number of inputs for this blockSystem
	void * storage; // put anything you want here
	updateChildInputsFunction updateChildInputs; // childInput = f(time, childStates, systemInput)
	struct block * child; // array of child blocks
	FLOAT_TYPE ** childInput; // array of pointers. one for each child input array
//	FLOAT_TYPE const ** childState; // array of pointers. one for each child state array
};

struct block * blockSystem( struct block * block, struct blockSystem * system );

#endif