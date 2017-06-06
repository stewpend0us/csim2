#pragma once
#include "blockSystemStorage.h"

struct StrictlyProperBlock blockSystem
(
	size_t const numSystemInputs,
	size_t const numSystemOutputs,
	struct BlockSystemStorage * const storage
);

struct StrictlyProperBlock * blockSystem_new
(
	size_t const numSystemInputs,
	size_t const numSystemOutputs,
	struct BlockSystemStorage * const storage
);

void blockSystem_free(struct StrictlyProperBlock * b);