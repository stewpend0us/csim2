#pragma once
#include "StrictlyProperBlock.h"

struct transferFunctionStorage
{
	size_t numSize;
	size_t denSize;
	double const * numerator;
	double const * denominator;
};

struct StrictlyProperBlock transferFunction(
	size_t const numBlocks,
	struct transferFunctionStorage * const storage,
	UtilityFunction const util
);