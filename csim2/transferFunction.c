#include <string.h>
#include <assert.h>
#include "transferFunction.h"

static void physics
(
	size_t const numStates,
	size_t const numInputs,
	double * const dState,
	double const time,
	double const * const state,
	double const * const input,
	struct transferFunctionStorage const * const storage
)
{
	size_t xi = 0;
	size_t nStates;
	for (size_t i = 0; i < numInputs; i++)
	{
		nStates = storage[i].denSize - 1;
		memcpy(&dState[xi], &state[xi + 1], nStates - 1); //dx1 = x2, dx2 = x3, ...
		
		dState[nStates - 1] = input[i];
		for (size_t j = 0; j < nStates; j++)
			dState[nStates - 1] -= storage[i].denominator[nStates - j] * state[xi + j];
		dState[nStates - 1] /= storage[i].denominator[0];
		xi += nStates;
	}
}

static void output
(
	size_t const numStates,
	size_t const numOutputs,
	double * const output,
	double const time,
	double const * const state,
	struct transferFunctionStorage const * const storage
)
{
	size_t xi = 0;
	for (size_t i = 0; i < numOutputs; i++)
	{
		output[i] = 0;
		for (size_t j = 0; j < storage[i].numSize; j++)
			output[i] = storage[i].numerator[storage[i].numSize - 1 - j] * state[xi + j];
	}
}

struct StrictlyProperBlock transferFunction(size_t const numBlocks, struct transferFunctionStorage * const storage)
{
	assert(numBlocks > 0);
	size_t totalStates = 0;
	for (size_t i = 0; i < numBlocks; i++)
	{
		assert(storage[i].numSize > 0);
		assert(storage[i].denSize > storage[i].numSize); // requirement of "strictly proper" systems
		assert(storage[i].denominator[0] != 0.0);
		totalStates += storage[i].denSize - 1;
	}

	struct StrictlyProperBlock b;
	b.numInputs = numBlocks;
	b.numOutputs = numBlocks;
	b.numStates = totalStates;
	b.storage = storage;
	b.f = physics;
	b.h = output;
	return b;
}