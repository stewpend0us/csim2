#include <string.h>
#include <errno.h>
#include "../csim2/constructors.h"
#include "../csim2/integrator.h"
#include "../csim2/firstOrderLag.h"
#include "../csim2/secondOrderSystem.h"
#include "../csim2/blockSystem.h"
#include "../csim2heap/blockSystemStorage_heap.h"
#include "../csim2heap/dllInterface.h"

// DEMO 1 =================================================================================

static struct StrictlyProperBlock * getInt(char const * const options)
{
	errno = 0;
	long numStates = strtol(options, NULL, 10); // convert the options string into an integer
	if (errno == ERANGE)
		return NULL;
	if (numStates <= 0)
		return NULL;
	return block_new(integrator(numStates));
}

__declspec(dllexport) struct dllStrictlyProperBlock Int1 = { getInt, block_free };

// DEMO 1 =================================================================================

// DEMO 2 =================================================================================

static struct StrictlyProperBlock * getFirstOrderLag1(char const * const options)
{
	errno = 0;
	double tau = strtod(options, NULL);
	if (errno == ERANGE)
		return NULL;
	if (tau <= 0)
		return NULL;

	double * taup = malloc(sizeof(double));
	if (!taup)
		return NULL;

	taup[0] = tau;

	return block_new(firstOrderLag(1, taup));
}

static void FirstOrderLag1_free(struct StrictlyProperBlock * block)
{
	free(block->storage);
	block_free(block);
}

__declspec(dllexport) struct dllStrictlyProperBlock FirstOrderLag1 = { getFirstOrderLag1, FirstOrderLag1_free };

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

	struct StrictlyProperBlock * I = block_new(integrator(1));
	struct BlockSystemStorage * Bs = blockSystemStorage_new(1, I, fol_blockInputs, fol_blockOutputs, taup);
	struct StrictlyProperBlock * B = block_new(blockSystem(1, 1, Bs));

	return B;
}

static void FirstOrderLag2_free(struct StrictlyProperBlock * block)
{
	struct BlockSystemStorage * Bs = block->storage;
	free(Bs->systemStorage);
	block_free(Bs->blocks);
	blockSystemStorage_free(Bs);
	block_free(block);
}

__declspec(dllexport) struct dllStrictlyProperBlock FirstOrderLag2 = { getFirstOrderLag2, FirstOrderLag2_free };

// DEMO 3 =================================================================================

// DEMO 4 =================================================================================

static struct StrictlyProperBlock * getSecondOrderSystem1(char const * const options)
{
	char * loc = strpbrk(options, ",");
	if (!loc)
		return NULL;

	errno = 0;
	double zeta = strtod(options, &loc);
	if (errno == ERANGE)
		return NULL;
	if (zeta <= 0)
		return NULL;

	errno = 0;
	double omega_n = strtod(loc+1, NULL);
	if (errno == ERANGE)
		return NULL;
	if (omega_n <= 0)
		return NULL;

	struct secondOrderSystemStorage * storagep = malloc(sizeof(struct secondOrderSystemStorage));
	if (!storagep)
		return NULL;

	storagep[0].zeta = zeta;
	storagep[0].omega_n = omega_n;

	return block_new(secondOrderSystem(1, storagep));
}

static void SecondOrderSystem1_free(struct StrictlyProperBlock * block)
{
	free(block->storage);
	block_free(block);
}

__declspec(dllexport) struct dllStrictlyProperBlock SecondOrderSystem1 = { getSecondOrderSystem1, SecondOrderSystem1_free };

// DEMO 4 =================================================================================