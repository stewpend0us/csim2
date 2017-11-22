#pragma once
#include "StrictlyProperBlock.h"
#include "ControllerBlock.h"

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
);

void rk4_f_step
(
	struct StrictlyProperBlockInfo const * const info,
	PhysicsFunction const f,
	double * const nextState, // (1 x numStates)
	double * const currentdState, // (1 x numStates)
	double * const B, // (1 x numStates) solver storage/temp
	double * const C, // (1 x numStates) solver storage/temp
	double * const D, // (1 x numStates) solver storage/temp
	double const dt, // time step
	double const currentTime, // current time
	double const * const currentState, // (1 x numStates) also initial conditions
	double const * const currentInput, // (1 x numInputs)
	double const * const currentInput2, // (1 x numInputs) input at time ti + dt/2
	double const * const nextInput // (1 x numInputs) input at time ti + dt
);

// euler solver
void euler
(
	struct StrictlyProperBlock const * const block,
	double * const Y, // numSteps x numOutputs output buffer
	double const dt, //time step
	size_t const numSteps,
	double const * const time, // numSteps x 1 time vector
	double const * const Xi, // numStates x 1 initial conditions vector (will be over-written)
	double const * const U // numSteps x numInputs input values over time
);

// rk4 solver
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