#include <assert.h>
#include "blockSystem.h"

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
		struct StrictlyProperBlock const B = blocks[i];
		B.f(B.numStates, B.numInputs, &dState[xi], time, &state[xi], blockInputs[i], B.storage);
		xi += B.numStates;
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
		struct StrictlyProperBlock const B = blocks[i];
		B.h(B.numStates, B.numOutputs, blockOutputs[i], time, &state[xi], B.storage);
		xi += B.numStates;
	}
	storage->calcSystemOutput(numOutputs, output, time, blockOutputs, systemStorage);
}

struct BlockSystemStorage blockSystemStorage_new
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
		struct StrictlyProperBlock const B = blocks[i];
		totalBlockInputs += B.numInputs;
		totalBlockOutputs += B.numOutputs;
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
		struct StrictlyProperBlock const B = blocks[i];
		input_storage[i] = &storage[inputi];
		output_storage[i] = &storage[outputi];
		inputi += B.numInputs;
		outputi += B.numOutputs;
	}

	return (struct BlockSystemStorage)
	{
		.numBlocks = numBlocks, .blocks = blocks, .blockInputs = input_storage, .blockOutputs = output_storage, .calcBlockInputs = calcBlockInputs, .calcSystemOutput = calcSystemOutputs, .systemStorage = systemStorage,
	};
}

void blockSystemStorage_free(struct BlockSystemStorage const * const storage)
{
	free(storage->blockInputs[0]);
	free(storage->blockInputs);
	free(storage->blockOutputs);
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

	return (struct StrictlyProperBlock)
	{
		.numStates = totalBlockStates, .numInputs = numSystemInputs, .numOutputs = numSystemOutputs, .f = blockSystem_physics, .h = blockSystem_output, .storage = storage,
	};
}