#include "singlestep.h"

void euler_step
(
	struct block const * block,
	double nextState[], // (1 x numStates)
	double dState[], // (1 x numStates)
	double dt,
	double time,
	double const state[], // (1 x numStates)
	double const input[] // (1 x numInputs)
)
{
	size_t i;
	size_t numStates = block->numStates;
	physicsFunction f = block->f;
	f(block, dState, time, state, input);
	for (i = 0; i < numStates; i++)
		nextState[i] = state[i] + dt * dState[i];
}

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
)
{
	size_t i;
	size_t numStates = block->numStates;
	physicsFunction f = block->f;
	double halfdt = dt / 2;
	double halfStepTime = time + halfdt;
	double nextTime = time + dt;

	f(block, dState, time, state, input);
	for (i = 0; i < numStates; i++)
		nextState[i] = state[i] + dt * dState[i];

	f(block, dB, halfStepTime, nextState, halfStepInput);
	for (i = 0; i < numStates; i++)
		nextState[i] = state[i] + halfdt * dB[i];

	f(block, dC, halfStepTime, nextState, halfStepInput);
	for (i = 0; i < numStates; i++)
		nextState[i] = state[i] + halfdt * dC[i];

	f(block, dD, nextTime, nextState, nextInput);
	for (i = 0; i < numStates; i++)
		nextState[i] = state[i] + dt * (dState[i] + 2*dB[i] + 2*dC[i] + dD[i]) / 6;
}
