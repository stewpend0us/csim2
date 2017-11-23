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
typedef void(*BlockSystemOutputFunction)(
	size_t const numSystemOutputs,
	double * const systemOutputs,
	double const time,
	double const * const * const blockOutputs,
	void * const systemStorage
	);

typedef void(*BlockSystemUtilityFunction)(
	double const time,
	size_t const numSystemInputs,
	double const * const systemInputs,
	size_t const numSystemOutputs,
	double const * const systemOutputs,
	void * const systemStorage
);

struct BlockSystemStorage
{
	size_t numBlocks;
	struct StrictlyProperBlock const * blocks;
	double * const * blockInputs;
	double * const * blockOutputs;
	CalcBlockInputsFunction calcBlockInputs;
	BlockSystemOutputFunction systemOutput;
	BlockSystemUtilityFunction systemUtility;
	void * systemStorage;
};

struct StrictlyProperBlock blockSystem
(
	size_t const numSystemInputs,
	size_t const numSystemOutputs,
	struct BlockSystemStorage * const storage
);