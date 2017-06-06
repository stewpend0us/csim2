#include "blockSystemStorage.h"
#include <assert.h>
#include <string.h>

struct BlockSystemStorage * blockSystemStorage_new
(
	size_t const numBlocks,
	struct StrictlyProperBlock * const blocks,
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

void blockSystemStorage_free(struct BlockSystemStorage * storage)
{
	free(storage->blockInputs[0]);
	free(storage->blockInputs);
	free(storage->blockOutputs);
	free(storage);
}