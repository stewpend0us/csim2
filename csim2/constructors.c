#include <string.h>
#include "constructors.h"

struct StrictlyProperBlock * block_new(struct StrictlyProperBlock const stackb)
{
	struct StrictlyProperBlock * heapb = malloc(sizeof(struct StrictlyProperBlock));
	if (heapb)
		memcpy(heapb, &stackb, sizeof(struct StrictlyProperBlock));
	return heapb;
}

void block_free(struct StrictlyProperBlock * heapb)
{
	free(heapb);
}

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
	struct BlockSystemStorage * const bheap = malloc(sizeof(struct BlockSystemStorage));

	if (storage == NULL || input_storage == NULL || output_storage == NULL || bheap == NULL)
	{
#define freeif(ptr) if (ptr) free(ptr)
		freeif(storage);
		freeif(input_storage);
		freeif(output_storage);
		freeif(bheap);
#undef freeif
		return NULL;
	}

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