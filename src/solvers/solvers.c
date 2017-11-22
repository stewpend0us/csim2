
#include <string.h>
#include "dbg.h"
#include "solvers.h"

void euler_f_step
(
	struct StrictlyProperBlockInfo const * const info,
	PhysicsFunction const f,
	double * const nextState, // (1 x numStates)
	double * const currentdState, // (1 x numStates)
	double const dt, // time step
	double const currentTime,
	double const * const currentState, // (1 x numStates) also initial conditions
	double const * const currentInput // (1 x numInputs)
)
{
	size_t i;
	f(info, currentdState, currentTime, currentState, currentInput);
	for (i = 0; i < info->numStates; i++)
		nextState[i] = currentState[i] + currentdState[i] * dt;
}

void rk4_f_step
(
	struct StrictlyProperBlockInfo const * const info,
	PhysicsFunction const f,
	double * const nextState, // (1 x numStates)
	double * const currentdState, // (1 x numStates)
	double * const B, // (1 x numStates) solve/temp
	double * const C, // (1 x numStates) solve/temp
	double * const D, // (1 x numStates) solve/temp
	double const dt, // time step
	double const currentTime, // current time
	double const * const currentState, // (1 x numStates) also initial conditions
	double const * const currentInput, // (1 x numInputs)
	double const * const currentInput2, // (1 x numInputs) input at time ti + dt/2
	double const * const nextInput // (1 x numInputs) input at time ti + dt
)
{
	size_t i;
	double const dt2 = dt / 2;
	double const currentTime2 = currentTime + dt2;
	double const nextTime = currentTime + dt;
	double * const A = currentdState;

	check_debug(currentState != nextState, "current state cannot be the same location in memory");
	check_debug(A != B && A != C && A != D && A != nextState && A != currentState, "B, C, D, nextState, and currentState cannot be the same location as A in memory");
	check_debug(B != C && B != D && B != nextState && B != currentState, "C, D, nextState, and currentState cannot be the same location as B in memory");
	check_debug(C != D && C != nextState && C != currentState, "D, C, nextState, and currentState cannot be the same location as C in memory");
	check_debug(D != nextState && D != currentState, "nextState, and currentState cannot be the same location as D in memory");

	size_t const numStates = info->numStates;

	f(info, A, currentTime, currentState, currentInput);
	for (i = 0; i < numStates; i++)
		nextState[i] = currentState[i] + A[i] * dt2;

	f(info, B, currentTime2, nextState, currentInput2);
	for (i = 0; i < numStates; i++)
		nextState[i] = currentState[i] + B[i] * dt2;

	f(info, C, currentTime2, nextState, currentInput2);
	for (i = 0; i < numStates; i++)
		nextState[i] = currentState[i] + C[i] * dt;

	f(info, D, nextTime, nextState, nextInput);
	for (i = 0; i < numStates; i++)
		nextState[i] = currentState[i] + dt * (A[i] + 2 * B[i] + 2 * C[i] + D[i]) / 6;
	
error:
	return;
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

		h(&bi, currentOutput, time[i], currentState);
		euler_f_step(&bi, f, nextState, currentdState, dt, time[i], currentState, currentInput);
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

		h(&bi, currentOutput, time[i], currentState);
		rk4_f_step(&bi, f, nextState, currentdState, B, C, D, dt, time[i], currentState, currentInput, currentInput2, nextInput);
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