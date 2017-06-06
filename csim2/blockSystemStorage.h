#pragma once
#include "StrictlyProperBlock.h"

// update blockInputs as a function of time, blockOutputs, and systemInputs
typedef void(*CalcBlockInputsFunction)(
	size_t const numBlocks,
	struct StrictlyProperBlock const * const blocks,
	double * const * const blockInputs,
	double const time,
	double const * const * const blockOutputs,
	size_t const numSystemInputs,
	double const * const systemInputs,
	void * const systemStorage
	);

// update systemOutputs as a function of time, and blockOutputs
typedef void(*CalcSystemOutputFunction)(
	size_t const numSystemOutputs,
	double * const systemOutputs,
	double const time,
	double const * const * const blockOutputs,
	void * const systemStorage
	);

struct BlockSystemStorage
{
	size_t numBlocks;
	struct StrictlyProperBlock * blocks;
	double ** blockInputs;
	double ** blockOutputs;
	CalcBlockInputsFunction calcBlockInputs;
	CalcSystemOutputFunction calcSystemOutput;
	void * systemStorage;
};

struct BlockSystemStorage * blockSystemStorage_new
(
	size_t const numBlocks,
	struct StrictlyProperBlock * const blocks,
	CalcBlockInputsFunction const calcBlockInputs,
	CalcSystemOutputFunction const calcSystemOutputs,
	void * const systemStorage
);

void blockSystemStorage_free(struct BlockSystemStorage * storage);