#ifndef _BLOCKSYSTEM_H_
#define _BLOCKSYSTEM_H_

#include "block.h"

struct blockSystem;

typedef void(*updateChildInputsFunction)(
	FLOAT_TYPE time,
	size_t num_children,
	struct block * child,
	FLOAT_TYPE * const child_input[],
	FLOAT_TYPE const * const child_state[],
	size_t num_inputs,
	FLOAT_TYPE const system_input[],
	void * storage
	);

struct blockSystem
{
	size_t num_children; // number of child blocks
	size_t num_inputs; // number of inputs for this blockSystem (not the total number of inputs of all the children)
	struct block * child; // array of child blocks
	FLOAT_TYPE ** child_state; // one pointer for each child (just need room for the pointers no data)
	FLOAT_TYPE ** child_input; // one pointer for each child (just need room for the pointers no data)
	void * storage; // put anything you want here
	updateChildInputsFunction updateChildInputs; // child_input = f(time, child_state, system_input)
};

struct block * blockSystem( struct block * block, struct blockSystem * system );

#endif
