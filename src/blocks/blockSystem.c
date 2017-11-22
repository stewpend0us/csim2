#include <assert.h>
#include <string.h>
#include "blockSystem.h"

// BlockSystem
static void physics
(
	struct StrictlyProperBlockInfo const * const info,
	double * const dState,
	double const time,
	double const * const state,
	double const * const input
)
{
	
	size_t const numInputs = info->numInputs;
	struct BlockSystemStorage const * const bss = info->storage;
	size_t const numBlocks = bss->numBlocks;
	struct StrictlyProperBlock const * const blocks = bss->blocks;
	double * const * const blockInputs = bss->blockInputs;
	double * const * const blockOutputs = bss->blockOutputs;
	void * const storage = bss->storage;

	// update all of the block outputs based on the latest state
	size_t xi = 0;
	struct StrictlyProperBlockInfo bi;
	for (size_t i = 0; i < numBlocks; i++)
	{
		bi = blocks[i].info;
		blocks[i].h(&bi, blockOutputs[i], time, &state[xi]);
		xi += bi.numStates;
	}
	// calculate the block inputs from the updated block outputs
	bss->calcBlockInputs(numBlocks, blocks, blockInputs, time, blockOutputs, numInputs, input, storage);
	// finally calculate the block dstate from the updated block inputs
	xi = 0;
	for (size_t i = 0; i < numBlocks; i++)
	{
		bi = blocks[i].info;
		blocks[i].f(&bi, &dState[xi], time, &state[xi], blockInputs[i]);
		xi += bi.numStates;
	}
}

static void output
(
	struct StrictlyProperBlockInfo const * const info,
	double * const output,
	double const time,
	double const * const state
)
{
	(void) numStates;

	size_t const numBlocks = storage->numBlocks;
	struct StrictlyProperBlock const * const blocks = storage->blocks;
	double * const * const blockOutputs = storage->blockOutputs;
	void * const storage = storage->storage;
	size_t xi = 0;
	for (size_t i = 0; i < numBlocks; i++)
	{
		blocks[i].h(blocks[i].numStates, blocks[i].numOutputs, blockOutputs[i], time, &state[xi], blocks[i].storage);
		xi += blocks[i].numStates;
	}
	storage->calcSystemOutput(numOutputs, output, time, blockOutputs, storage);
}


struct StrictlyProperBlock blockSystem
(
	size_t const numSystemInputs,
	size_t const numSystemOutputs,
	struct BlockSystemStorage * const storage
)
{
	size_t const numBlocks = storage->numBlocks;
	struct StrictlyProperBlock const * const blocks = storage->blocks;

	size_t totalBlockStates = 0;
	for (size_t i = 0; i < numBlocks; i++)
		totalBlockStates += blocks[i].numStates;

	struct StrictlyProperBlock bsstack;
	bsstack.numStates = totalBlockStates;
	bsstack.numInputs = numSystemInputs;
	bsstack.numOutputs = numSystemOutputs;
	bsstack.f = (PhysicsFunction)physics;
	bsstack.h = (OutputFunction)output;
	bsstack.storage = storage;
	return bsstack;
}