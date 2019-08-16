#include <dbg.h>
#include "transferFunction.h"

static void physics
(
	FLOAT_TYPE time,
	size_t num_states,
	FLOAT_TYPE dState[],
	FLOAT_TYPE const state[],
	size_t num_inputs,
	FLOAT_TYPE input[],
	void * storage
)
{
	(void)time;
	struct transferFunctionStorage * tf_storage = block->storage;
	size_t xi = 0;
	size_t n_states;
	for (size_t i = 0; i < num_inputs; i++)
	{
		n_states = tf_storage[i].den_size - 1;
		memcpy(&dState[xi], &state[xi + 1], n_states - 1); //dx1 = x2, dx2 = x3, ...
		
		dState[n_states - 1] = input[i];
		for (size_t j = 0; j < n_states; j++)
			dState[n_states - 1] -= tf_storage[i].denominator[n_states - j] * state[xi + j];
		dState[n_states - 1] /= tf_storage[i].denominator[0];
		xi += n_states;
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
		for (size_t j = 0; j < tf_storage[i].num_size; j++)
			output[i] = tf_storage[i].numerator[tf_storage[i].num_size - 1 - j] * state[xi + j];
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
		check(tf_storage[i].num_size > 0, "tf_storage[%zu].num_size must be greater than zero", i);
		check(tf_storage[i].den_size > tf_storage[i].num_size, "tf_storage[%zu].den_size must be greater than tf_storage[%zu].num_size",i,i); // requirement of "strictly proper" systems
		check(tf_storage[i].denominator[0] != 0.0, "tf_storage[%zu].denominator[0] must not be zero", i);
		totalStates += tf_storage[i].den_size - 1;
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
