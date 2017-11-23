
#include <string.h>
#include "dbg.h"
#include "solvers.h"
	
void euler_step
(
	struct StrictlyProperBlockInfo const * const info,
	OutputFunction const h,
	PhysicsFunction const f,
	double * const nextState, // (1 x numStates)
	double * const dState, // (1 x numStates)
	double * const output, // (1 x numOutputs)
	double const dt,
	double const time,
	double const * const state, // (1 x numStates)
	double const * const input // (1 x numInputs)
)
{
	size_t const numStates = info->numStates;
	h(info, output, time, state);
	f(info, dState, time, state, input);
	for (size_t i = 0; i < numStates; i++)
		nextState[i] = state[i] + dState[i] * dt;
}

/* not sure where this should live
	check_debug(currentState != nextState, "current state cannot be the same location in memory");
	check_debug(dA != B && dA != C && dA != D && dA != nextState && dA != currentState, "B, C, D, nextState, and currentState cannot be the same location as dA in memory");
	check_debug(B != C && B != D && B != nextState && B != currentState, "C, D, nextState, and currentState cannot be the same location as B in memory");
	check_debug(C != D && C != nextState && C != currentState, "D, C, nextState, and currentState cannot be the same location as C in memory");
	check_debug(D != nextState && D != currentState, "nextState, and currentState cannot be the same location as D in memory");
*/

void rk4_step
(
	struct StrictlyProperBlockInfo const * const info,
	OutputFunction const h,
	PhysicsFunction const f,
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
	double * const dA = dState; //for consistency
	size_t const numStates = info->numStates;
	double const halfdt = dt / 2;
	double const halfStepTime = time + halfdt;
	double const nextTime = time + dt;

	h(info, output, time, state);
	f(info, dA, time, state, input);
	for (i = 0; i < numStates; i++)
		nextState[i] = state[i] + dA[i] * dt;

	f(info, dB, halfStepTime, nextState, halfStepInput);
	for (i = 0; i < numStates; i++)
		nextState[i] = state[i] + dB[i] * halfdt;

	f(info, dC, halfStepTime, nextState, halfStepInput);
	for (i = 0; i < numStates; i++)
		nextState[i] = state[i] + dC[i] * halfdt;

	f(info, dD, nextTime, nextState, nextInput);
	for (i = 0; i < numStates; i++)
		nextState[i] = state[i] + dt * (dA[i] + 2*dB[i] + 2*dC[i] + dD[i]) / 6;
}

void euler
(
	struct StrictlyProperBlock const * const block,
	double * const Y, // numSteps x numOutputs output buffer
	double const dt, //time step
	size_t const numSteps,
	double const * const time, // numSteps x 1 time vector
	double const * const Xi, // numStates x 1 initial conditions vector (will be over-written)
	double const * const U // numSteps x numInputs input values over time
)
{
	struct StrictlyProperBlockInfo const bi = block->info;
	size_t const numStates = bi.numStates;
	size_t const numInputs = bi.numInputs;
	size_t const numOutputs = bi.numOutputs;
	OutputFunction const h = block->h;
	PhysicsFunction const f = block->f;

	double * const temp_memory = malloc(numStates * 2 * sizeof(double));
	check_mem(temp_memory);
	
	double * const currentState = &temp_memory[0 * numStates];
	double * const nextState = currentState; //in this case it's ok that these are the same block of memory
	double * const currentdState = &temp_memory[1 * numStates];
	
	double const * currentInput;
	double * currentOutput;

	memcpy(currentState, Xi, numStates * sizeof(double));

	size_t i;
	for (i = 0; i < (numSteps - 1); i++)
	{
		currentInput = &U[i*numInputs];
		currentOutput = &Y[i*numOutputs];

		euler_step(&bi, h, f, nextState, currentdState, currentOutput, dt, time[i], currentState, currentInput);
	}

	//currentState = nextState;
	currentOutput = &Y[i*numOutputs];
	h(&bi, currentOutput, time[i], currentState);

error:
	free(temp_memory);
}

void rk4
(
	struct StrictlyProperBlock const * const block,
	double * const Y,
	double const dt, //time step
	size_t numSteps, //size of time vector
	double const * const time, //time vector
	double const * const Xi, // numStates x 1 initial conditions vector
	double const * const U1, // numSteps x numInputs inputs
	double const * const U2 // numSteps x numInputs inputs
)
{
	struct StrictlyProperBlockInfo const bi = block->info;
	size_t const numStates = bi.numStates;
	size_t const numInputs = bi.numInputs;
	size_t const numOutputs = bi.numOutputs;
	OutputFunction const h = block->h;
	PhysicsFunction const f = block->f;

	double * const temp_memory = malloc(numStates * 6 * sizeof(double));
	check_mem(temp_memory);

	double * const currentState = &temp_memory[0 * numStates];
	double * const nextState = &temp_memory[1 * numStates];
	double * const currentdState = &temp_memory[2 * numStates];
	double * const B = &temp_memory[3 * numStates];
	double * const C = &temp_memory[4 * numStates];
	double * const D = &temp_memory[5 * numStates];

	double const * currentInput;
	double const * currentInput2;
	double const * nextInput;
	double * currentOutput;

	memcpy(currentState, Xi, numStates * sizeof(double));
	size_t i;
	for (i = 0; i < (numSteps - 1); i++)
	{
		currentInput = &U1[i*numInputs];
		currentInput2 = &U2[i*numInputs];
		nextInput = &U1[(i + 1)*numInputs];
		currentOutput = &Y[i*numOutputs];

		rk4_step(&bi, h, f, nextState, currentdState, B, C, D, currentOutput, dt, time[i], currentState, currentInput, currentInput2, nextInput);
		memcpy(currentState, nextState, numStates * sizeof(double));
	}

	currentOutput = &Y[i*numOutputs];
	h(&bi, currentOutput, time[i], currentState);
error:
	free(temp_memory);
}



#include <math.h>

size_t numTimeSteps
(
	double const dt,
	double const duration
)
{
	return (size_t)floor(duration / dt + 1);
}

void initializeTime
(
	size_t const numSteps,
	double * const time,
	double const dt,
	double const ti
)
{
	for (size_t i = 0; i < numSteps; i++)
		time[i] = i*dt + ti;
}