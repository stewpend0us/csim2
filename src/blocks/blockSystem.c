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
	(void)numStates;

	size_t numBlocks = storage->numBlocks;
	struct StrictlyProperBlock const * const blocks = storage->blocks;
	double * const * const blockInputs = storage->blockInputs;
	double * const * const blockOutputs = storage->blockOutputs;
	void * const systemStorage = storage->systemStorage;

	// update all of the block outputs based on the latest state
	size_t xi = 0;
	for (size_t i = 0; i < numBlocks; i++)
	{
		blocks[i].h(blocks[i].numStates, blocks[i].numOutputs, blockOutputs[i], time, &state[xi], blocks[i].storage);
		xi += blocks[i].numStates;
	}
	// calculate the block inputs from the updated block outputs
	storage->calcBlockInputs(numBlocks, blocks, blockInputs, time, blockOutputs, numInputs, input, systemStorage);
	// finally calculate the block dstate from the updated block inputs
	xi = 0;
	for (size_t i = 0; i < numBlocks; i++)
	{
		blocks[i].f(blocks[i].numStates, blocks[i].numInputs, &dState[xi], time, &state[xi], blockInputs[i], blocks[i].storage);
		xi += blocks[i].numStates;
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
	void * const systemStorage = storage->systemStorage;
	size_t xi = 0;
	for (size_t i = 0; i < numBlocks; i++)
	{
		blocks[i].h(blocks[i].numStates, blocks[i].numOutputs, blockOutputs[i], time, &state[xi], blocks[i].storage);
		xi += blocks[i].numStates;
	}
	storage->calcSystemOutput(numOutputs, output, time, blockOutputs, systemStorage);
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

	struct StrictlyProperBlock bstack;
	bstack.numStates = totalBlockStates;
	bstack.numInputs = numSystemInputs;
	bstack.numOutputs = numSystemOutputs;
	bstack.f = (PhysicsFunction)physics;
	bstack.h = (OutputFunction)output;
	bstack.storage = storage;
	return bstack;
}