#include <assert.h>
#include <string.h>
#include "blockSystem.h"




// BlockSystem
static void blockSystem_physics
(
	size_t const numStates,
	size_t const numInputs,
	double * const dState,
	double const time,
	double const * const state,
	double const * const input,
	struct BlockSystemStorage * const storage
)
{
	size_t numBlocks = storage->numBlocks;
	struct StrictlyProperBlock const * const blocks = storage->blocks;
	double * const * const blockInputs = storage->blockInputs;
	double const * const * const blockOutputs = storage->blockOutputs;
	void * const systemStorage = storage->systemStorage;
	size_t xi = 0;
	storage->calcBlockInputs(numBlocks, blocks, blockInputs, time, blockOutputs, numInputs, input, systemStorage);
	for (size_t i = 0; i < numBlocks; i++)
	{
		blocks[i].f(blocks[i].numStates, blocks[i].numInputs, &dState[xi], time, &state[xi], blockInputs[i], blocks[i].storage);
		xi += blocks[i].numStates;
	}
}

static void blockSystem_output
(
	size_t const numStates,
	size_t const numOutputs,
	double * const output,
	double const time,
	double const * const state,
	struct BlockSystemStorage * const storage
)
{
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
	bstack.f = blockSystem_physics;
	bstack.h = blockSystem_output;
	bstack.storage = storage;
	return bstack;
}

struct StrictlyProperBlock * blockSystem_new
(
	size_t const numSystemInputs,
	size_t const numSystemOutputs,
	struct BlockSystemStorage * const storage
)
{
	struct StrictlyProperBlock stackb = blockSystem(numSystemInputs, numSystemOutputs, storage);
	struct StrictlyProperBlock * heapb = malloc(sizeof(struct StrictlyProperBlock));
	memcpy(heapb, &stackb, sizeof(struct StrictlyProperBlock));
	return heapb;
}

void blockSystem_free(struct StrictlyProperBlock * b)
{
	free(b);
}