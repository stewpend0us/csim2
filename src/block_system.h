#ifndef _BLOCK_SYSTEM_H_
#define _BLOCK_SYSTEM_H_

#include "block.h"

struct block_system;

typedef void(*update_child_inputs_function)(
	FLOAT_TYPE time,
	size_t num_children,
	struct block * child,
	FLOAT_TYPE * const child_input[],
	FLOAT_TYPE const * const child_state[],
	size_t num_inputs,
	FLOAT_TYPE const system_input[],
	void * storage
	);

struct block_system
{
	size_t num_children; // number of child blocks
	size_t num_inputs; // number of inputs for this block_system (not the total number of inputs of all the children)
	struct block * child; // array of child blocks
	FLOAT_TYPE ** child_state; // one pointer for each child (just need room for the pointers no data)
	FLOAT_TYPE ** child_input; // one pointer for each child (just need room for the pointers no data)
	void * storage; // put anything you want here
	update_child_inputs_function update_child_inputs; // child_input = f(time, child_state, system_input)
};

struct block * block_system( struct block * block, struct block_system * system );

#endif
