#include "dbg.h"
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
	double const * const * const blockOutputs_const = (double const * const * const)blockOutputs; //this gets around a warning
	void * const storage = bss->systemStorage;

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
	bss->calcBlockInputs(numBlocks, blocks, blockInputs, time, blockOutputs_const, numInputs, input, storage);
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
	struct BlockSystemStorage const * const bss = info->storage;
	size_t const numOutputs = info->numOutputs;
	size_t const numBlocks = bss->numBlocks;
	struct StrictlyProperBlock const * const blocks = bss->blocks;
	double * const * const blockOutputs = bss->blockOutputs;
	double const * const * const blockOutputs_const = (double const * const * const)blockOutputs; //this gets around a warning

	size_t xi = 0;
	for (size_t i = 0; i < numBlocks; i++)
	{
		struct StrictlyProperBlockInfo bi = blocks[i].info;
		blocks[i].h(&bi, blockOutputs[i], time, &state[xi]);
		xi += bi.numStates;
	}
	void * const storage = bss->systemStorage;
	bss->systemOutput(numOutputs, output, time, blockOutputs_const, storage);
}

static void util
(
	struct StrictlyProperBlockInfo const * const info,	
	double const time,
	double const * const dState,
	double const * const state,
	double const * const input,
	double const * const output
)
{
	struct BlockSystemStorage const * const bss = info->storage;
	size_t const numBlocks = bss->numBlocks;
	struct StrictlyProperBlock const * const blocks = bss->blocks;
	double const * const * const blockInputs = (double const * const * const)bss->blockInputs;
	double const * const * const blockOutputs = (double const * const * const)bss->blockOutputs;
	
	if (bss->systemUtility)
		bss->systemUtility(time, info->numInputs, input, info->numOutputs, output, bss->systemStorage);
	
	size_t xi = 0;
	for (size_t i = 0; i < numBlocks; i++)
	{
		struct StrictlyProperBlockInfo bi = blocks[i].info;
		if (blocks[i].u)
			blocks[i].u(&bi, time, &dState[xi], &state[xi], blockInputs[i], blockOutputs[i]);
		xi += bi.numStates;
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
		totalBlockStates += blocks[i].info.numStates;
	}
	return (struct StrictlyProperBlock)
	{
		{
			totalBlockStates,
			numSystemInputs,
			numSystemOutputs,
			storage,
		},
		output,
		physics,
		util,
	};
error:
	return NULL_StritclyProperBlock;
}