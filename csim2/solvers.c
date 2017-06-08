#include <math.h>
#include <assert.h>
#include "solvers.h"

void euler_f_step
(
	size_t const numStates,
	size_t const numInputs,
	double const dt, // time step
	double const currentTime,
	double * const nextState, // (1 x numStates)
	double * const currentdState, // (1 x numStates)
	double const * const currentState, // (1 x numStates) also initial conditions
	double const * const currentInput, // (1 x numInputs)
	PhysicsFunction const f,
	void * const storage
)
{
	size_t i;
	f(numStates, numInputs, currentdState, currentTime, currentState, currentInput, storage);
	for (i = 0; i < numStates; i++)
		nextState[i] = currentState[i] + currentdState[i] * dt;
}

void rk4_f_step
(
	size_t const numStates,
	size_t const numInputs,
	double const dt, // time step
	double const currentTime, // current time
	double * const nextState, // (1 x numStates)
	double * const currentdState, // (1 x numStates)
	double * const B, // (1 x numStates) solver storage/temp
	double * const C, // (1 x numStates) solver storage/temp
	double * const D, // (1 x numStates) solver storage/temp
	double const * const currentState, // (1 x numStates) also initial conditions
	double const * const currentInput, // (1 x numInputs)
	double const * const currentInput2, // (1 x numInputs) input at time ti + dt/2
	double const * const nextInput, // (1 x numInputs) input at time ti + dt
	PhysicsFunction const f,
	void * const storage
)
{
	size_t i;
	double const dt2 = dt / 2;
	double const currentTime2 = currentTime + dt2;
	double const nextTime = currentTime + dt;
	double * const A = currentdState;

	assert(currentState != nextState);
	assert(A != B && A != C && A != D && A != nextState && A != currentState);
	assert(B != C && B != D && B != nextState && B != currentState);
	assert(C != D && C != nextState && C != currentState);
	assert(D != nextState && D != currentState);

	f(numStates, numInputs, A, currentTime, currentState, currentInput, storage);
	for (i = 0; i < numStates; i++)
		nextState[i] = currentState[i] + A[i] * dt2;

	f(numStates, numInputs, B, currentTime2, nextState, currentInput2, storage);
	for (i = 0; i < numStates; i++)
		nextState[i] = currentState[i] + B[i] * dt2;

	f(numStates, numInputs, C, currentTime2, nextState, currentInput2, storage);
	for (i = 0; i < numStates; i++)
		nextState[i] = currentState[i] + C[i] * dt;

	f(numStates, numInputs, D, nextTime, nextState, nextInput, storage);
	for (i = 0; i < numStates; i++)
		nextState[i] = currentState[i] + dt * (A[i] + 2 * B[i] + 2 * C[i] + D[i]) / 6;
}

//size_t numSteps
//(
//	double const ti,
//	double const dt,
//	double const tf
//)
//{
//	return (size_t)floor((tf - ti) / dt + 1);
//}