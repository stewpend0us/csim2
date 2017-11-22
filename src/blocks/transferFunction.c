#include <string.h>
#include <assert.h>
#include "transferFunction.h"

static void physics
(
	struct StrictlyProperBlockInfo const * const info,
	double * const dState,
	double const time,
	double const * const state,
	double const * const input
)
{
	(void)numStates;
	(void)time;
	struct transferFunctionStorage const * const tf_storage = storage;
	size_t xi = 0;
	size_t nStates;
	for (size_t i = 0; i < numInputs; i++)
	{
		nStates = tf_storage[i].denSize - 1;
		memcpy(&dState[xi], &state[xi + 1], nStates - 1); //dx1 = x2, dx2 = x3, ...
		
		dState[nStates - 1] = input[i];
		for (size_t j = 0; j < nStates; j++)
			dState[nStates - 1] -= tf_storage[i].denominator[nStates - j] * state[xi + j];
		dState[nStates - 1] /= tf_storage[i].denominator[0];
		xi += nStates;
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
	(void)numStates;
	(void)time;
	struct transferFunctionStorage const * const tf_storage = storage;

	size_t xi = 0;
	for (size_t i = 0; i < numOutputs; i++)
	{
		output[i] = 0;
		for (size_t j = 0; j < tf_storage[i].numSize; j++)
			output[i] = tf_storage[i].numerator[tf_storage[i].numSize - 1 - j] * state[xi + j];
	}
}

struct StrictlyProperBlock transferFunction(size_t const numBlocks, struct transferFunctionStorage * const tf_storage)
{
	assert(numBlocks > 0);
	size_t totalStates = 0;
	for (size_t i = 0; i < numBlocks; i++)
	{
		assert(tf_storage[i].numSize > 0);
		assert(tf_storage[i].denSize > tf_storage[i].numSize); // requirement of "strictly proper" systems
		assert(tf_storage[i].denominator[0] != 0.0);
		totalStates += tf_storage[i].denSize - 1;
	}

	struct StrictlyProperBlock b;
	b.numInputs = numBlocks;
	b.numOutputs = numBlocks;
	b.numStates = totalStates;
	b.storage = tf_storage;
	b.f = physics;
	b.h = output;
	return b;
}