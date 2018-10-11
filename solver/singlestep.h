#ifndef _SINGLESTEP_H_
#define _SINGLESTEP_H_

#include "block.h"

void euler_step
(
	struct block const * block,
	double nextState[], // (1 x numStates)
	double dState[], // (1 x numStates)
	double dt,
	double time,
	double const state[], // (1 x numStates)
	double const input[] // (1 x numInputs)
);

void rk4_step
(
	struct block const * block,
	double nextState[], // (1 x numStates)
	double dState[], // (1 x numStates)
	double dB[], // (1 x numStates) temp
	double dC[], // (1 x numStates) temp
	double dD[], // (1 x numStates) temp
	double dt,
	double time,
	double const state[], // (1 x numStates)
	double const input[], // (1 x numInputs)
	double const halfStepInput[], // (1 x numInputs)
	double const nextInput[] // (1 x numInputs)
);

#endif
