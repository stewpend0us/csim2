#include <assert.h>
#include "blockSystem.h"

struct BlockSystemStorage blockSystemStorage_new
(
	size_t const numBlocks,
	struct StrictlyProperBlock const * const blocks,
	CalcBlockInputsFunction const calcInputs,
	CalcSystemOutputFunction const calcOutputs
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

	return (struct BlockSystemStorage)
	{
		.numBlocks = numBlocks, .blocks = blocks, .blockInputs = input_storage, .blockOutputs = output_storage, .updateInputs = calcInputs, .calcOutput = calcOutputs,
	};
}

void blockSystemStorage_free(struct BlockSystemStorage const * const storage)
{
	free(storage->blockInputs[0]);
	free(storage->blockInputs);
	free(storage->blockOutputs);
}

static void blockSystem_physics
(
	size_t const numDstates,
	double * const dState,
	double const time,
	size_t const numStates,
	double const * const state,
	size_t const numInputs,
	double const * const input,
	struct BlockSystemStorage * const storage
)
{
	size_t numBlocks = storage->numBlocks;
	struct StrictlyProperBlock const * const blocks = storage->blocks;
	double * const * const blockInputs = storage->blockInputs;
	double const * const * const blockOutputs = storage->blockOutputs;
	size_t xi = 0;
	storage->updateInputs(numBlocks, blocks, blockInputs, time, blockOutputs, numInputs, input);
	for (size_t i = 0; i < numBlocks; i++)
	{
		blocks[i].f(blocks[i].numStates, &dState[xi], time, blocks[i].numStates, &state[xi], blocks[i].numInputs, blockInputs[i], blocks[i].storage);
		xi += blocks[i].numStates;
	}
}

static void blockSystem_output
(
	size_t const numOutputs,
	double * const output,
	double const time,
	size_t const numStates,
	double const * const state,
	struct BlockSystemStorage * const storage
)
{
	size_t numBlocks = storage->numBlocks;
	struct StrictlyProperBlock const * const blocks = storage->blocks;
	double * const * const blockOutputs = storage->blockOutputs;
	size_t xi = 0;
	for (size_t i = 0; i < numBlocks; i++)
	{
		blocks[i].h(blocks[i].numOutputs, blockOutputs[i], time, blocks[i].numStates, &state[xi], blocks[i].storage);
		xi += blocks[i].numStates;
	}
	storage->calcOutput(numOutputs, output, time, blockOutputs);
}

struct StrictlyProperBlock blockSystem(size_t const numInputs, size_t const numOutputs, struct BlockSystemStorage * const storage)
{
	size_t totalBlockStates = 0;
	for (size_t i = 0; i < storage->numBlocks; i++)
	{
		totalBlockStates += storage->blocks[i].numStates;
	}

	return (struct StrictlyProperBlock)
	{
		.numStates = totalBlockStates, .numInputs = numInputs, .numOutputs = numOutputs, .f = blockSystem_physics, .h = blockSystem_output, .storage = storage,
	};
}