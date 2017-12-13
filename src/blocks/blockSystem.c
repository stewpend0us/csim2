#include "dbg.h"
#include "blockSystem.h"

// BlockSystem
static void physics
(
	struct StrictlyProperBlock const * const block,
	double * const dState,
	double const time,
	double const * const state,
	double const * const input
)
{
	
	size_t const numInputs = block->numInputs;
	struct BlockSystemStorage const * const bss = block->storage;
	size_t const numBlocks = bss->numBlocks;
	struct StrictlyProperBlock const * const blocks = bss->blocks;
	double * const * const blockInputs = bss->blockInputs;
	double * const * const blockOutputs = bss->blockOutputs;
	double const * const * const blockOutputs_const = (double const * const * const)blockOutputs; //this gets around a warning
	void * const storage = bss->systemStorage;

	// update all of the block outputs based on the latest state
	size_t xi = 0;
	for (size_t i = 0; i < numBlocks; i++)
	{
		blocks[i].h(&blocks[i], blockOutputs[i], time, &state[xi]);
		xi += blocks[i].numStates;
	}
	// calculate the block inputs from the updated block outputs
	bss->calcBlockInputs(numBlocks, blocks, blockInputs, time, blockOutputs_const, numInputs, input, storage);
	// finally calculate the block dstate from the updated block inputs
	xi = 0;
	for (size_t i = 0; i < numBlocks; i++)
	{
		blocks[i].f(&blocks[i], &dState[xi], time, &state[xi], blockInputs[i]);
		xi += blocks[i].numStates;
	}
}

static void output
(
	struct StrictlyProperBlock const * const block,
	double * const output,
	double const time,
	double const * const state
)
{
	struct BlockSystemStorage const * const bss = block->storage;
	size_t const numOutputs = block->numOutputs;
	size_t const numBlocks = bss->numBlocks;
	struct StrictlyProperBlock const * const blocks = bss->blocks;
	double * const * const blockOutputs = bss->blockOutputs;
	double const * const * const blockOutputs_const = (double const * const * const)blockOutputs; //this gets around a warning

	size_t xi = 0;
	for (size_t i = 0; i < numBlocks; i++)
	{
		blocks[i].h(&blocks[i], blockOutputs[i], time, &state[xi]);
		xi += blocks[i].numStates;
	}
	void * const storage = bss->systemStorage;
	bss->systemOutput(numOutputs, output, time, blockOutputs_const, storage);
}

static void util
(
	struct StrictlyProperBlock const * const block,	
	double const time,
	double const * const dState,
	double const * const state,
	double const * const input,
	double const * const output
)
{
	struct BlockSystemStorage const * const bss = block->storage;
	size_t const numBlocks = bss->numBlocks;
	struct StrictlyProperBlock const * const blocks = bss->blocks;
	double const * const * const blockInputs = (double const * const * const)bss->blockInputs;
	double const * const * const blockOutputs = (double const * const * const)bss->blockOutputs;
	
	if (bss->systemUtility)
		bss->systemUtility(time, block->numInputs, input, block->numOutputs, output, bss->systemStorage);
	
	size_t xi = 0;
	for (size_t i = 0; i < numBlocks; i++)
	{
		if (blocks[i].u)
			blocks[i].u(&blocks[i], time, &dState[xi], &state[xi], blockInputs[i], blockOutputs[i]);
		xi += blocks[i].numStates;
	}
}

struct StrictlyProperBlock blockSystem
(
	size_t const numSystemInputs,
	size_t const numSystemOutputs,
	struct BlockSystemStorage * const storage
)
{
	check(storage, "storage must not be NULL");
	size_t const numBlocks = storage->numBlocks;
	struct StrictlyProperBlock const * const blocks = storage->blocks;

	size_t totalBlockStates = 0;
	for (size_t i = 0; i < numBlocks; i++)
	{
		totalBlockStates += blocks[i].numStates;
	}
	return (struct StrictlyProperBlock)
	{
		totalBlockStates,
		numSystemInputs,
		numSystemOutputs,
		storage,
		output,
		physics,
		util,
	};
error:
	return NULL_StritclyProperBlock;
}