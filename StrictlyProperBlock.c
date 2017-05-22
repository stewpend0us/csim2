#include <assert.h>
#include <string.h>
#include "StrictlyProperBlock.h"

static void integrator_physics(size_t const numDstates, double * const dState, double const t, size_t const numStates, double const * const state, size_t const numInputs, double const * const input, void * const storage)
{
	assert(numStates == numInputs);
	memcpy(dState, input, numStates * sizeof(double));
}

static void integrator_output(size_t const numOutputs, double * const output, double const t, size_t const numStates, double const * const state, void * const storage)
{
	assert(numStates == numOutputs);
	memcpy(output, state, numStates * sizeof(double));
}

struct StrictlyProperBlock integrator(size_t const numSignals)
{
	return (struct StrictlyProperBlock)
	{
		.numStates = numSignals, .numInputs = numSignals, .numOutputs = numSignals, .f = integrator_physics, .h = integrator_output, .storage = NULL,
	};
}

struct SystemStorage system_storage_new(size_t const numBlocks, struct StrictlyProperBlock const * const blocks, UpdateBlockInputsFunction const updateInputs, CalcSystemOutputFunction const calcOutput)
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
		.numBlocks = numBlocks, .blocks = blocks, .blockInputs = input_storage, .blockOutputs = output_storage, .updateInputs = updateInputs, .calcOutput = calcOutput,
	};
}

void system_storage_free(struct SystemStorage const * const storage)
{
	free(storage->blockInputs[0]);
	free(storage->blockInputs);
	free(storage->blockOutputs);
}

static void blockSystem_physics(size_t const numDstates, double * const dState, double const t, size_t const numStates, double const * const state, size_t const numInputs, double const * const input, struct SystemStorage * const storage)
{
	size_t numBlocks = storage->numBlocks;
	struct StrictlyProperBlock const * const blocks = storage->blocks;
	double * const * const blockInputs = storage->blockInputs;
	double const * const * const blockOutputs = storage->blockOutputs;
	size_t xi = 0;
	storage->updateInputs(numBlocks, blocks, blockInputs, blockOutputs, numInputs, input);
	for (size_t i = 0; i < numBlocks; i++)
	{
		blocks[i].f(blocks[i].numStates, &dState[xi], t, blocks[i].numStates, &state[xi], blocks[i].numInputs, blockInputs[i], blocks[i].storage);
		xi += blocks[i].numStates;
	}
}

static void blockSystem_output(size_t const numOutputs, double * const output, double const t, size_t const numStates, double const * const state, struct SystemStorage * const storage)
{
	size_t numBlocks = storage->numBlocks;
	struct StrictlyProperBlock const * const blocks = storage->blocks;
	double * const * const blockOutputs = storage->blockOutputs;
	size_t xi = 0;
	for (size_t i = 0; i < numBlocks; i++)
	{
		blocks[i].h(blocks[i].numOutputs, blockOutputs[i], t, blocks[i].numStates, &state[xi], blocks[i].storage);
		xi += blocks[i].numStates;
	}
	storage->calcOutput(numOutputs, output, blockOutputs);
}

struct StrictlyProperBlock blockSystem(size_t const numInputs, size_t const numOutputs, struct SystemStorage * const storage)
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

void rk4(double * const Y, double * const Xi, struct StrictlyProperBlock const * const block, size_t const num_t, double const * const t, double const * const U)
{

	size_t yi = block->numOutputs;
	size_t xi = block->numStates;
	void * storage = block->storage;

	for (size_t i = 0; i < num_t; i++)
	{
		block->h(yi, &Y[i*yi], t[i], xi, Xi, storage);

	}
	//for i = 1:numpts - 1
	//	[A, output(i, :)] = f.physics(t(i), state(i, :), input(i, :));

	//if endEarly(i, t, state, output)
	//	beenEnded = true;
	//break;
	//end
	//	B = f.physics(t2(i), state(i, :) + A*dt2, input2(i, :));
	//C = f.physics(t2(i), state(i, :) + B*dt2, input2(i, :));
	//D = f.physics(t(i + 1), state(i, :) + C*dt, input(i + 1, :));

	//state(i + 1, :) = state(i, :) + dt*(A + 2 * B + 2 * C + D) / 6;
	//dstate(i, :) = A;

	//if any(isnan(state(i, :))) && ~beenWarned
	//	warning('state contains NaN at time %g', t(i));
	//beenWarned = true;
	//end
	//	end
}