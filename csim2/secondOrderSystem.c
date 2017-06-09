#include "secondOrderSystem.h"

static void physics
(
	size_t const numStates,
	size_t const numInputs,
	double * const dState,
	double const time,
	double const * const state,
	double const * const input,
	struct secondOrderSystemStorage const * const storage
)
{
	double zeta;
	double wn;
	double wn2;
	size_t i1;
	size_t i2;
	for (size_t i = 0; i < numInputs; i++)
	{
		i1 = i * 2;
		i2 = i1 + 1;
		zeta = storage[i].zeta;
		wn = storage[i].omega_n;
		wn2 = wn*wn;

		dState[i1] = state[i2];
		dState[i2] = -2*zeta*wn*state[i2] - wn2*state[i1] + wn2*input[i];
	}
}

static void output
(
	size_t const numStates,
	size_t const numOutputs,
	double * const output,
	double const time,
	double const * const state,
	struct secondOrderSystemStorage const * const storage
)
{
	size_t i1;
	for (size_t i = 0; i < numOutputs; i++)
	{
		i1 = i * 2;
		output[i] = state[i1];
	}
}

struct StrictlyProperBlock secondOrderSystem(size_t const numBlocks, struct secondOrderSystemStorage * storage)
{
	struct StrictlyProperBlock b;
	b.numInputs = numBlocks;
	b.numOutputs = numBlocks;
	b.numStates = numBlocks*2;
	b.storage = storage;
	b.f = physics;
	b.h = output;
	return b;
}