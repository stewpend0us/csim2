#pragma once
#include "../csim2/secondOrderSystem.h"

struct StrictlyProperBlock * secondOrderSystem_new(size_t const numBlocks, struct secondOrderSystemStorage * const storage);
void secondOrderSystem_free(struct StrictlyProperBlock * b);