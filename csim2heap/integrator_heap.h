#pragma once
#include "../csim2/integrator.h"

struct StrictlyProperBlock * integrator_new(size_t const numSignals);
void integrator_free(struct StrictlyProperBlock * b);