
#include "dbg.h"
#include "singlestep.h"

void euler_step
(
	struct StrictlyProperBlock const * const block,
	double * const nextState, // (1 x numStates)
	double * const dState, // (1 x numStates)
	double * const output, // (1 x numOutputs)
	double const dt,
	double const time,
	double const * const state, // (1 x numStates)
	double const * const input // (1 x numInputs)
)
{
	size_t const numStates = block->numStates;
	OutputFunction const h = block->h;
	PhysicsFunction const f = block->f;
	UtilityFunction const u = block->u;
	h(block, output, time, state);
	f(block, dState, time, state, input);
	if (u)
		u(block, time, dState, state, input, output);
	for (size_t i = 0; i < numStates; i++)
		nextState[i] = state[i] + dState[i] * dt;
}

/* not sure where this should live, should be an equivelant in the euler solver
	check_debug(currentState != nextState, "current state cannot be the same location in memory");
	check_debug(dA != B && dA != C && dA != D && dA != nextState && dA != currentState, "B, C, D, nextState, and currentState cannot be the same location as dA in memory");
	check_debug(B != C && B != D && B != nextState && B != currentState, "C, D, nextState, and currentState cannot be the same location as B in memory");
	check_debug(C != D && C != nextState && C != currentState, "D, C, nextState, and currentState cannot be the same location as C in memory");
	check_debug(D != nextState && D != currentState, "nextState, and currentState cannot be the same location as D in memory");
*/

void rk4_step
(
	struct StrictlyProperBlock const * const block,
	double * const nextState, // (1 x numStates)
	double * const dState, // (1 x numStates)
	double * const dB, // (1 x numStates) temp
	double * const dC, // (1 x numStates) temp
	double * const dD, // (1 x numStates) temp
	double * const output, // (1 x numOutputs)
	double const dt,
	double const time,
	double const * const state, // (1 x numStates)
	double const * const input, // (1 x numInputs)
	double const * const halfStepInput, // (1 x numInputs)
	double const * const nextInput // (1 x numInputs)
)
{
	size_t i;
	size_t const numStates = block->numStates;
	OutputFunction const h = block->h;
	PhysicsFunction const f = block->f;
	UtilityFunction const u = block->u;
	double const halfdt = dt / 2;
	double const halfStepTime = time + halfdt;
	double const nextTime = time + dt;

	h(block, output, time, state);
	f(block, dState, time, state, input);
	if (u)
		u(block, time, dState, state, input, output);
	
	for (i = 0; i < numStates; i++)
		nextState[i] = state[i] + dState[i] * halfdt;
	f(block, dB, halfStepTime, nextState, halfStepInput);
	
	for (i = 0; i < numStates; i++)
		nextState[i] = state[i] + dB[i] * halfdt;
	f(block, dC, halfStepTime, nextState, halfStepInput);
	
	for (i = 0; i < numStates; i++)
		nextState[i] = state[i] + dC[i] * dt;
	f(block, dD, nextTime, nextState, nextInput);
	
	for (i = 0; i < numStates; i++)
		nextState[i] = state[i] + dt * (dState[i] + 2*dB[i] + 2*dC[i] + dD[i]) / 6;
}
