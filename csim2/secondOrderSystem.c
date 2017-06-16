#include <string.h>
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
	double * const state1 = state;
	double * const state2 = state + numInputs;
	double * const dState1 = dState;
	double * const dState2 = dState + numInputs;

	memcpy(dState1, state2, numInputs * sizeof(double));

	for (size_t i = 0; i < numInputs; i++)
		dState2[i] = -2 * storage[i].zeta * storage[i].omega_n * state2[i] - storage[i].omega_n * storage[i].omega_n * ( state1[i] - input[i] );
	
	//double zeta;
	//double wn;
	//double wn2;
	//size_t i1;
	//size_t i2;
	//for (size_t i = 0; i < numInputs; i++)
	//{
	//	i1 = i * 2;
	//	i2 = i1 + 1;
	//	zeta = storage[i].zeta;
	//	wn = storage[i].omega_n;
	//	wn2 = wn*wn;

	//	dState[i1] = state[i2];
	//	dState[i2] = -2*zeta*wn*state[i2] - wn2*state[i1] + wn2*input[i];
	//}
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
	double * const state1 = state;
	memcpy(output, state1, numOutputs * sizeof(double));

	//size_t i1;
	//for (size_t i = 0; i < numOutputs; i++)
	//{
	//	i1 = i * 2;
	//	output[i] = state[i1];
	//}
}

struct StrictlyProperBlock secondOrderSystem(size_t const numBlocks, struct secondOrderSystemStorage * const storage)
{
	struct StrictlyProperBlock b;
	b.numInputs = numBlocks;
	b.numOutputs = numBlocks;
	b.numStates = 2 * numBlocks;
	b.storage = storage;
	b.f = physics;
	b.h = output;
	return b;
}