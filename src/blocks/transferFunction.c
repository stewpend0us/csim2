#include <dbg.h>
#include "transferFunction.h"

static void physics
(
	struct StrictlyProperBlock const * const block,
	double * const dState,
	double const time,
	double const * const state,
	double const * const input
)
{
	(void)time;
	size_t const numInputs = block->numInputs;

	struct transferFunctionStorage const * const tf_storage = block->storage;
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
	struct StrictlyProperBlock const * const block,
	double * const output,
	double const time,
	double const * const state
)
{
	(void)time;
	size_t const numOutputs = block->numOutputs;
	struct transferFunctionStorage const * const tf_storage = block->storage;

	size_t xi = 0;
	for (size_t i = 0; i < numOutputs; i++)
	{
		output[i] = 0;
		for (size_t j = 0; j < tf_storage[i].numSize; j++)
			output[i] = tf_storage[i].numerator[tf_storage[i].numSize - 1 - j] * state[xi + j];
	}
}

struct StrictlyProperBlock transferFunction(
	size_t const numBlocks,
	struct transferFunctionStorage * const tf_storage,
	UtilityFunction const util
)
{
	check(numBlocks > 0, "numBlocks must be greater than zero");
	check(tf_storage, "tf_storage must not be NULL");

	size_t totalStates = 0;
	for (size_t i = 0; i < numBlocks; i++)
	{
		check(tf_storage[i].numSize > 0, "tf_storage[%zu].numSize must be greater than zero", i);
		check(tf_storage[i].denSize > tf_storage[i].numSize, "tf_storage[%zu].denSize must be greater than tf_storage[%zu].numSize",i,i); // requirement of "strictly proper" systems
		check(tf_storage[i].denominator[0] != 0.0, "tf_storage[%zu].denominator[0] must not be zero", i);
		totalStates += tf_storage[i].denSize - 1;
	}
	return (struct StrictlyProperBlock)
	{
		totalStates,
		numBlocks,
		numBlocks,
		tf_storage,
		output,
		physics,
		util,
	};
error:
	return NULL_StritclyProperBlock;
}