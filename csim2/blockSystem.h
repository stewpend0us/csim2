#pragma once
#include <stdlib.h>
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


struct StrictlyProperBlock * blockSystem_new
(
	size_t const numSystemInputs,
	size_t const numSystemOutputs,
	size_t const numBlocks,
	struct StrictlyProperBlock const * const blocks,
	CalcBlockInputsFunction const calcBlockInputs,
	CalcSystemOutputFunction const calcSystemOutputs,
	void * const systemStorage
);

void blockSystem_free(struct StrictlyProperBlock * const blockSystem);