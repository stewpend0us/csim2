#include <assert.h>
#include <string.h>
#include "../csim2/integrator.h"
#include "../csim2/blockSystem.h"

// few simple cases of just an integrator
__declspec(dllexport) struct StrictlyProperBlock getInt1()
{
	return integrator(1);
}

__declspec(dllexport) struct StrictlyProperBlock getInt2()
{
	return integrator(2);
}

__declspec(dllexport) struct StrictlyProperBlock getInt10()
{
	return integrator(10);
}

// direct implementation of 1/(tau*s + 1)
static double Gtau = 3;

static void fol_physics
(
	size_t numStates,
	size_t numInputs,
	double * const dState,
	double const time,
	double const * const state,
	double const * const input,
	double const * const tau
)
{
	dState[0] = (input[0] - state[0]) / tau[0];
}

static void fol_output
(
	size_t numStates,
	size_t numOutputs,
	double * const output,
	double const time,
	double const * const state,
	void const * const tau
)
{
	assert(numStates == numOutputs);
	memcpy(output, state, numStates * sizeof(double));
}

__declspec(dllexport) struct StrictlyProperBlock getFirstOrderLag1()
{
	return (struct StrictlyProperBlock)
	{
		.numStates = 1, .numInputs = 1, .numOutputs = 1, .f = fol_physics, .h = fol_output, .storage = &Gtau,
	};
}

// blockSystem implementation of 1/(tau*s + 1)
void fol_blockInputs
(
	size_t const numBlocks,
	struct StrictlyProperBlock const * const blocks,
	double * const * const blockInputs,
	double const time,
	double const * const * const blockOutputs,
	size_t const numSystemInputs,
	double const * const systemInputs,
	double const * const tau
)
{
	blockInputs[0][0] = (systemInputs[0] - blockOutputs[0][0]) / tau[0];
}

void fol_blockOutputs
(
	size_t const numSystemOutputs,
	double * const systemOutputs,
	double const time,
	double const * const * const blockOutputs,
	double const * const tau
)
{
	memcpy(systemOutputs, blockOutputs[0], numSystemOutputs * sizeof(double));
}

__declspec(dllexport) struct StrictlyProperBlock getFirstOrderLag2()
{
	struct StrictlyProperBlock temp = integrator(1);
	struct StrictlyProperBlock * blocks = malloc(sizeof(struct StrictlyProperBlock));
	memcpy(blocks, &temp, sizeof(struct StrictlyProperBlock));

	struct BlockSystemStorage temp2 = blockSystemStorage_new(1, blocks, fol_blockInputs, fol_blockOutputs, &Gtau);
	struct BlockSystemStorage * storage = malloc(sizeof(struct BlockSystemStorage));
	memcpy(storage, &temp2, sizeof(struct BlockSystemStorage));

	return blockSystem(1, 1, storage);
}