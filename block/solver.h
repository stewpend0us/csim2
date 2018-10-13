#ifndef _SOLVER_H_
#define _SOLVER_H_

#include "block.h"

void euler
(
	struct block const * block,
	FLOAT_TYPE nextState[], // (1 x numStates)
	FLOAT_TYPE dState[], // (1 x numStates)
	FLOAT_TYPE dt,
	FLOAT_TYPE time,
	FLOAT_TYPE const state[], // (1 x numStates)
	FLOAT_TYPE const input[] // (1 x numInputs)
);

void rk4
(
	struct block const * block,
	FLOAT_TYPE nextState[], // (1 x numStates)
	FLOAT_TYPE dState[], // (1 x numStates)
	FLOAT_TYPE dB[], // (1 x numStates) temp
	FLOAT_TYPE dC[], // (1 x numStates) temp
	FLOAT_TYPE dD[], // (1 x numStates) temp
	FLOAT_TYPE dt,
	FLOAT_TYPE time,
	FLOAT_TYPE const state[], // (1 x numStates)
	FLOAT_TYPE const input[], // (1 x numInputs)
	FLOAT_TYPE const halfStepInput[], // (1 x numInputs)
	FLOAT_TYPE const nextInput[] // (1 x numInputs)
);

#endif
