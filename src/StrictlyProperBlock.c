#include "StrictlyProperBlock.h"

void outputState
(
	struct StrictlyProperBlock const * const block,
	double * const output,
	double const time,
	double const * const state
)
{
	(void)time;
	size_t const numStates = block->numStates;	
	memcpy(output, state, numStates * sizeof(double));
}