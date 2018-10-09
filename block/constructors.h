#pragma once
#include "StrictlyProperBlock.h"
#include "blockSystem.h"

struct StrictlyProperBlock * block_new(struct StrictlyProperBlock const stackb);
void block_free(struct StrictlyProperBlock * heapb);

struct BlockSystemStorage * blockSystemStorage_new
(
	size_t const numBlocks,
	struct StrictlyProperBlock const * const blocks,
	CalcBlockInputsFunction const calcBlockInputs,
	BlockSystemOutputFunction const systemOutputs,
	BlockSystemUtilityFunction const systemUtility,
	void * const systemStorage
);

void blockSystemStorage_free(struct BlockSystemStorage * storage);