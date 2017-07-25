#pragma once
#include "StrictlyProperBlock.h"

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
);

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
);

// euler solver
void euler
(
	struct StrictlyProperBlock block,
	double const dt, //time step
	size_t numSteps, //size of time vector
	double const * const time, //time vector
	size_t numStates,
	double const * const Xi, // numStates x 1 initial conditions vector
	size_t numInputs,
	double const * const U, // numSteps x numInputs inputs
	size_t numOutputs,
	double * const Y
);

// rk4 solver
void rk4(
	struct StrictlyProperBlock block,
	double const dt, //time step
	size_t numSteps, //size of time vector
	double const * const time, //time vector
	size_t numStates,
	double const * const Xi, // numStates x 1 initial conditions vector
	size_t numInputs,
	double const * const U1, // numSteps x numInputs inputs
	double const * const U2, // numSteps x numInputs inputs
	size_t numOutputs,
	double * const Y
);

// other:
size_t numTimeSteps
(
	double const dt,
	double const duration
);

void initializeTime
(
	size_t const numSteps,
	double * const time,
	double const dt,
	double const ti	
);