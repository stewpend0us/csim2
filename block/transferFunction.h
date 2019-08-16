#pragma once
#include "StrictlyProperBlock.h"

struct transferFunctionStorage
{
	size_t num_size;
	size_t den_size;
	double const * numerator;
	double const * denominator;
};

struct StrictlyProperBlock transferFunction(
	size_t const numBlocks,
	struct transferFunctionStorage * const storage,
	UtilityFunction const util
);
