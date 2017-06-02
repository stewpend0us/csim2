#include "../csim2/integrator.h"
#include "../csim2/blockSystem.h"

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

static void fol_physics
(
	size_t const numDstates,
	double * const dState,
	double const time,
	size_t const numStates,
	double const * const state,
	size_t const numInputs,
	double const * const input,
	double const * const tau
)
{
	dState[0] = (input[0] - state[0]) / tau[0];
}

static void fol_output
(
	size_t const numOutputs,
	double * const output,
	double const time,
	size_t const numStates,
	double const * const state,
	void const * const tau
)
{
	memcpy(output, state, numStates * sizeof(double));
}

static double const tau = 3;
__declspec(dllexport) struct StrictlyProperBlock getFirstOrderLag1()
{
	return (struct StrictlyProperBlock)
	{
		.numStates = 1, .numInputs = 1, .numOutputs = 1, .f = fol_physics, .h = fol_output, .storage = &tau,
	};
}


static struct BlockSystemStorage const storage = (struct BlockSystemStorage)
{
	.numBlocks = 1, .blocks =
};

__declspec(dllexport) struct StrictlyProperBlock getFirstOrderLag2()
{
	struct BlockSystemStorage * const storage = malloc(sizeof(struct BlockSystemStorage));
	storage->numBlocks = 1;
	storage->blocks = malloc(1 * sizeof(struct StrictlyProperBlock));
	storage->blocks[0] = integrator(1);

	return blockSystem(1, 1, storage);
}