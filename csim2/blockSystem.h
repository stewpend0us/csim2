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
	double const * const systemInputs
	);

// update systemOutputs as a function of time, and blockOutputs
typedef void(*CalcSystemOutputFunction)(
	size_t const numSystemOutputs,
	double * const systemOutputs,
	double const time,
	double const * const * const blockOutputs
	);

// BlockSystemStorage data
struct BlockSystemStorage
{
	size_t const numBlocks;
	struct StrictlyProperBlock const * const blocks;
	double * const * const blockInputs;
	double * const * const blockOutputs;
	CalcBlockInputsFunction const updateInputs;
	CalcSystemOutputFunction const calcOutput;
};

// BlockSystemStorage constructor
struct BlockSystemStorage blockSystemStorage_new
(
	size_t const numBlocks,
	struct StrictlyProperBlock const * const blocks,
	CalcBlockInputsFunction const calcInputs,
	CalcSystemOutputFunction const calcOutputs
);

// BlockSystemStorage destructor
void blockSystemStorage_free(struct BlockSystemStorage const * const storage);

// blockSystem constructor
struct StrictlyProperBlock blockSystem(size_t const numInputs, size_t const numOutputs, struct BlockSystemStorage * const storage);