#pragma once
#include "StrictlyProperBlock.h"

struct secondOrderSystemStorage
{
	double zeta;
	double omega_n;
};

struct StrictlyProperBlock secondOrderSystem(size_t const numBlocks, struct secondOrderSystemStorage * const storage);