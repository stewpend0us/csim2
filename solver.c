#include "solver.h"

void euler
(
	struct block const * block,
	FLOAT_TYPE nextState[],
	FLOAT_TYPE dState[],
	FLOAT_TYPE dt,
	FLOAT_TYPE time,
	FLOAT_TYPE const state[],
	FLOAT_TYPE const input[]
)
{
	size_t i;
	size_t numStates = block->numStates;
	physicsFunction f = block->f;

	f(block, dState, time, state, input);
	for (i = 0; i < numStates; i++)
		nextState[i] = state[i] + dt * dState[i];
}

void rk4
(
	struct block const * block,
	FLOAT_TYPE nextState[],
	FLOAT_TYPE dState[],
	FLOAT_TYPE dB[],
	FLOAT_TYPE dC[],
	FLOAT_TYPE dD[],
	FLOAT_TYPE dt,
	FLOAT_TYPE time,
	FLOAT_TYPE const state[],
	FLOAT_TYPE const input[],
	FLOAT_TYPE const halfInput[],
	FLOAT_TYPE const nextInput[]
)
{
	size_t i;
	size_t numStates = block->numStates;
	physicsFunction f = block->f;
	FLOAT_TYPE halfdt = dt / 2;
	FLOAT_TYPE halfTime = time + halfdt;
	FLOAT_TYPE nextTime = time + dt;

	f(block, dState, time, state, input);
	for (i = 0; i < numStates; i++)
		nextState[i] = state[i] + dt * dState[i];

	f(block, dB, halfTime, nextState, halfInput);
	for (i = 0; i < numStates; i++)
		nextState[i] = state[i] + halfdt * dB[i];

	f(block, dC, halfTime, nextState, halfInput);
	for (i = 0; i < numStates; i++)
		nextState[i] = state[i] + halfdt * dC[i];

	f(block, dD, nextTime, nextState, nextInput);
	for (i = 0; i < numStates; i++)
		nextState[i] = state[i] + dt * (dState[i] + 2*dB[i] + 2*dC[i] + dD[i]) / 6;
}
