#include <assert.h>
#include <string.h>
#include "StrictlyProperBlock.h"

static void integrator_f(double * const dState, double t, double const * const state, double const * const input, size_t const numStates, size_t const numInputs, void * const storage)
{
	assert(numStates == numInputs);
	memcpy(dState, input, numStates * sizeof(double));
}

static void integrator_h(double * const output, double t, double const * const state, size_t numStates, size_t const numOutputs, void * const storage)
{
	assert(numStates == numOutputs);
	memcpy(output, state, numStates * sizeof(double));
}

struct StrictlyProperBlock integrator(size_t numSignals)
{
	return (struct StrictlyProperBlock)
	{
		.numStates = numSignals, .numInputs = numSignals, .numOutputs = numSignals, .f = integrator_f, .h = integrator_h, .storage = NULL,
	};
}

struct SystemStorage system_storage_new(size_t numBlocks, struct StrictlyProperBlock const * const blocks, void(*updateInputs)(double * const * const inputs, double const * const * const outputs))
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

	return (struct SystemStorage)
	{
		.numBlocks = numBlocks, .blocks = blocks, .blockInputs = input_storage, .blockOutputs = output_storage, .updateInputs = updateInputs,
	};
}

void system_storage_free(struct SystemStorage const * const storage)
{
	free(storage->blockInputs[0]);
	free(storage->blockInputs);
	free(storage->blockOutputs);
}

static void system_f(double * const dState, double t, double const * const state, double const * const input, size_t const numStates, size_t const numInputs, void * const storage)
{

}

static void system_h(double * const output, double t, double const * const state, size_t numStates, size_t const numOutputs, void * const storage)
{

}

struct StrictlyProperBlock blockSystem(size_t numInputs, size_t numOutputs, struct SystemStorage * const storage)
{
	size_t totalBlockStates = 0;
	for (size_t i = 0; i < storage->numBlocks; i++)
	{
		totalBlockStates += storage->blocks[i].numStates;
	}


	return (struct StrictlyProperBlock)
	{
		.numStates = totalBlockStates, .numInputs = numInputs, .numOutputs = numOutputs, .f = system_f, .h = system_h, .storage = storage,
	};
}
