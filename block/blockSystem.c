#include "blockSystem.h"

static void physics
(
	struct block const * block,
	FLOAT_TYPE dState[],
	FLOAT_TYPE time,
	FLOAT_TYPE const state[],
	FLOAT_TYPE const input[]
)
{
	
//	size_t const numInputs = block->numInputs;
//	struct BlockSystemStorage const * const bss = block->storage;
//	size_t const numBlocks = bss->numBlocks;
//	struct StrictlyProperBlock const * const blocks = bss->blocks;
//	double * const * const blockInputs = bss->blockInputs;
//	double * const * const blockOutputs = bss->blockOutputs;
//	double const * const * const blockOutputs_const = (double const * const * const)blockOutputs; //this gets around a warning
//	void * const storage = bss->systemStorage;
//
//	// calculate the block inputs from the updated block outputs
//	bss->calcBlockInputs(numBlocks, blocks, blockInputs, time, blockState, numInputs, input, storage);
//	// finally calculate the block dstate from the updated block inputs
//	xi = 0;
//	for (size_t i = 0; i < numBlocks; i++)
//	{
//		blocks[i].f(&blocks[i], &dState[xi], time, &state[xi], blockInputs[i]);
//		xi += blocks[i].numStates;
//	}
}

struct block * blockSystem( struct block * block, struct blockSystem * system )
{
	if ( !block || !system || !system->numChildren || !system->child || !system->childInput )
		return NULL;

	size_t numChildren = system->numChildren;
	struct block * child = system->child;
	FLOAT_TYPE ** childInput = system->childInput;

	size_t numStates = 0;
	for (size_t i = 0; i < numChildren; i++)
	{
		if ( child[i].numStates == 0 || childInput[i] == NULL )
			return NULL;
		numStates += child[i].numStates;
	}
	block->numStates = numStates;
	block->numInputs = system->numInputs;
	block->storage = system;
	block->f = physics;
	return block;
}
