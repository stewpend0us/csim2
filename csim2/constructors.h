#pragma once
#include "StrictlyProperBlock.h"

struct StrictlyProperBlock * block_new(struct StrictlyProperBlock const stackb);
void block_free(struct StrictlyProperBlock * heapb);