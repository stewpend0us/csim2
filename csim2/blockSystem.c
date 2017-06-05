#include <assert.h>
#include <string.h>
#include "blockSystem.h"

// BlockSystemStorage
struct BlockSystemStorage
{
	size_t numBlocks;
	struct StrictlyProperBlock const * blocks;
	double * const * blockInputs;
	double * const * blockOutputs;
	CalcBlockInputsFunction calcBlockInputs;
	CalcSystemOutputFunction calcSystemOutput;
	void * systemStorage;
};

struct BlockSystemStorage * const blockSystemStorage_new
(
	size_t const numBlocks,
	struct StrictlyProperBlock const * const blocks,
	CalcBlockInputsFunction const calcBlockInputs,
	CalcSystemOutputFunction const calcSystemOutputs,
	void * const systemStorage
)
{
	size_t totalBlockInputs = 0;
	size_t totalBlockOutputs = 0;
	for (size_t i = 0; i < numBlocks; i++)
	{
		totalBlockInputs += blocks[i].numInputs;
		totalBlockOutputs += blocks[i].numOutputs;
	}

	double * storage = malloc((totalBlockInputs + totalBlockOutputs) * sizeof(double));
	double ** input_storage = malloc(numBlocks * sizeof(double*));
	double ** output_storage = malloc(numBlocks * sizeof(double*));
	assert(storage != NULL);
	assert(input_storage != NULL);
	assert(output_storage != NULL);

	size_t inputi = 0;
	size_t outputi = totalBlockInputs;
	for (size_t i = 0; i < numBlocks; i++)
	{
		input_storage[i] = &storage[inputi];
		output_storage[i] = &storage[outputi];
		inputi += blocks[i].numInputs;
		outputi += blocks[i].numOutputs;
	}

	struct BlockSystemStorage bstack;
	bstack.numBlocks = numBlocks;
	bstack.blocks = blocks;
	bstack.blockInputs = input_storage;
	bstack.blockOutputs = output_storage;
	bstack.calcBlockInputs = calcBlockInputs;
	bstack.calcSystemOutput = calcSystemOutputs;
	bstack.systemStorage = systemStorage;

	struct BlockSystemStorage * const bheap = malloc(sizeof(struct BlockSystemStorage));
	memcpy(bheap, &bstack, sizeof(struct BlockSystemStorage));
	return bheap;
}

void blockSystemStorage_free(struct BlockSystemStorage * const storage)
{
	free(storage->blockInputs[0]);
	free(storage->blockInputs);
	free(storage->blockOutputs);
	free(storage);
}

// BlockSystem
static void blockSystem_physics
(
	size_t numStates,
	size_t numInputs,
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
	size_t numStates,
	size_t numOutputs,
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













struct StrictlyProperBlock * blockSystem_new
(
	size_t const numSystemInputs,
	size_t const numSystemOutputs,
	size_t const numBlocks,
	struct StrictlyProperBlock const * const blocks,
	CalcBlockInputsFunction const calcBlockInputs,
	CalcSystemOutputFunction const calcSystemOutputs,
	void * const systemStorage
)
{
	size_t totalBlockStates = 0;
	for (size_t i = 0; i < numBlocks; i++)
		totalBlockStates += blocks[i].numStates;

	struct StrictlyProperBlock bstack;
	bstack.numStates = totalBlockStates;
	bstack.numInputs = numSystemInputs;
	bstack.numOutputs = numSystemOutputs;
	bstack.f = blockSystem_physics;
	bstack.h = blockSystem_output;
	bstack.storage = blockSystemStorage_new( numBlocks, blocks, calcBlockInputs, calcSystemOutputs, systemStorage );
}

void blockSystem_free(struct StrictlyProperBlock * const b)
{
	blockSystemStorage_free(b->storage);
	free(b);
}