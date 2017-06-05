#pragma once

#include "StrictlyProperBlock.h"

struct StrictlyProperBlock integrator(size_t const numSignals);
struct StrictlyProperBlock * integrator_new(size_t const numSignals);
void integrator_free(struct StrictlyProperBlock * const b);