#ifndef _SOLVER_H_
#define _SOLVER_H_

#include "block.h"

void euler
(
	struct block const * block,
	FLOAT_TYPE next_state[], // (1 x numStates)
	FLOAT_TYPE dstate[], // (1 x numStates)
	FLOAT_TYPE dt,
	FLOAT_TYPE * time,
	FLOAT_TYPE const state[], // (1 x numStates)
	FLOAT_TYPE input[] // (1 x numInputs)
);

void rk4
(
	struct block const * block,
	FLOAT_TYPE next_state[], // (1 x numStates)
	FLOAT_TYPE dstate[], // (1 x numStates)
	FLOAT_TYPE dB[], // (1 x numStates) temp
	FLOAT_TYPE dC[], // (1 x numStates) temp
	FLOAT_TYPE dD[], // (1 x numStates) temp
	FLOAT_TYPE dt,
	FLOAT_TYPE * time,
	FLOAT_TYPE const state[], // (1 x numStates)
	FLOAT_TYPE input[], // (1 x numInputs)
	FLOAT_TYPE half_step_input[], // (1 x numInputs)
	FLOAT_TYPE next_input[] // (1 x numInputs)
);

#endif
