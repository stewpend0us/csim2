#pragma once
#include "integrator.h"
#include "firstOrderLag.h"
#include "secondOrderSystem.h"
#include "transferFunction.h"
#include "blockSystem.h"

struct StrictlyProperBlock * integrator_new(size_t const numBlocks);
void integrator_free(struct StrictlyProperBlock * b);

struct StrictlyProperBlock * firstOrderLag_new(size_t const numBlocks, double * const tau);
void firstOrderLag_free(struct StrictlyProperBlock * b);

struct StrictlyProperBlock * secondOrderSystem_new(size_t const numBlocks, struct secondOrderSystemStorage * const storage);
void secondOrderSystem_free(struct StrictlyProperBlock * b);

struct StrictlyProperBlock * transferFunction_new(size_t const numBlocks, struct transferFunctionStorage * const storage);
void transferFunction_free(struct StrictlyProperBlock * b);

struct StrictlyProperBlock * blockSystem_new
(
	size_t const numSystemInputs,
	size_t const numSystemOutputs,
	struct BlockSystemStorage * const storage
);
void blockSystem_free(struct StrictlyProperBlock * b);