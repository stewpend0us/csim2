#pragma once
#include "../csim2/transferFunction.h"

struct StrictlyProperBlock * transferFunction_new(size_t const numBlocks, struct transferFunctionStorage * const storage);
void transferFunction_free(struct StrictlyProperBlock * b);