#pragma once
#include "../csim2/blockSystemStorage.h"

struct BlockSystemStorage * blockSystemStorage_new
(
	size_t const numBlocks,
	struct StrictlyProperBlock * const blocks,
	CalcBlockInputsFunction const calcBlockInputs,
	CalcSystemOutputFunction const calcSystemOutputs,
	void * const systemStorage
);

void blockSystemStorage_free(struct BlockSystemStorage * storage);