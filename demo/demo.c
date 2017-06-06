#include <string.h>
#include <errno.h>
#include "../csim2/integrator.h"
#include "../csim2/blockSystem.h"
#include "../csim2/dllInterface.h"

// DEMO 1 =================================================================================

static struct StrictlyProperBlock * getInt(char const * const options)
{
	errno = 0;
	long numStates = strtol(options, NULL, 10); // convert the options string into an integer
	if (errno == ERANGE)
		return NULL;
	if (numStates <= 0)
		return NULL;
	return integrator_new(numStates);
}

__declspec(dllexport) struct dllStrictlyProperBlock Int1 = { getInt, integrator_free };

// DEMO 1 =================================================================================

// DEMO 2 =================================================================================

// direct implementation of 1/(tau*s + 1)
static void fol_physics
(
	size_t const numStates,
	size_t const numInputs,
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
	size_t const numStates,
	size_t const numOutputs,
	double * const output,
	double const time,
	double const * const state,
	void const * const tau
)
{
	memcpy(output, state, numStates * sizeof(double));
}

static struct StrictlyProperBlock * getFirstOrderLag1(char const * const options)
{
	errno = 0;
	double tau = strtod(options, NULL);
	if (errno == ERANGE)
		return NULL;
	if (tau <= 0)
		return NULL;

	double * taup = malloc(sizeof(double));
	taup[0] = tau;

	struct StrictlyProperBlock bstack;
	bstack.numStates = 1;
	bstack.numInputs = 1;
	bstack.numOutputs = 1;
	bstack.f = fol_physics;
	bstack.h = fol_output;
	bstack.storage = taup;

	struct StrictlyProperBlock * bheap = malloc(sizeof(struct StrictlyProperBlock));
	memcpy(bheap, &bstack, sizeof(struct StrictlyProperBlock));
	return bheap;
}

static void freeFirstOrderLag1(struct StrictlyProperBlock * block)
{
	free(block->storage);
	free(block);
}

__declspec(dllexport) struct dllStrictlyProperBlock FirstOrderLag1 = { getFirstOrderLag1, freeFirstOrderLag1 };

// DEMO 2 =================================================================================

// DEMO 3 =================================================================================

// blockSystem implementation of 1/(tau*s + 1)
static void fol_blockInputs
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

static void fol_blockOutputs
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

static struct StrictlyProperBlock * getFirstOrderLag2(char const * const options)
{
	errno = 0;
	double tau = strtod(options, NULL);
	if (errno == ERANGE)
		return NULL;
	if (tau <= 0)
		return NULL;

	double * taup = malloc(sizeof(double));
	taup[0] = tau;

	struct StrictlyProperBlock * I = integrator_new(1);
	struct BlockSystemStorage * Bs = blockSystemStorage_new(1, I, fol_blockInputs, fol_blockOutputs, taup);
	struct StrictlyProperBlock * B = blockSystem_new(1, 1, Bs);

	return B;
}

static void freeFirstOrderLag2(struct StrictlyProperBlock * block)
{
	struct BlockSystemStorage * Bs = block->storage;
	free(Bs->systemStorage);
	integrator_free(Bs->blocks);
	blockSystemStorage_free(Bs);
	blockSystem_free(block);
}

__declspec(dllexport) struct dllStrictlyProperBlock FirstOrderLag2 = { getFirstOrderLag2, freeFirstOrderLag2 };

// DEMO 3 =================================================================================