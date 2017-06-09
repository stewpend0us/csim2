#pragma once
#include "../csim2/integrator.h"

struct StrictlyProperBlock * integrator_new(size_t const numBlocks);
void integrator_free(struct StrictlyProperBlock * b);