#pragma once
#include "../csim2/firstOrderLag.h"

struct StrictlyProperBlock * firstOrderLag_new(size_t const numBlocks, double * const tau);
void firstOrderLag_free(struct StrictlyProperBlock * b);