#pragma once
#include "StrictlyProperBlock.h"
#include "blockSystem.h"

struct StrictlyProperBlock * block_new(struct StrictlyProperBlock const stackb);
void block_free(struct StrictlyProperBlock * heapb);

struct BlockSystemStorage * blockSystemStorage_new
(
	size_t const numBlocks,
	struct StrictlyProperBlock * const blocks,
	CalcBlockInputsFunction const calcBlockInputs,
	CalcSystemOutputFunction const calcSystemOutputs,
	void * const systemStorage
);

void blockSystemStorage_free(struct BlockSystemStorage * storage);