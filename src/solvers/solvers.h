#pragma once
#include "StrictlyProperBlock.h"
#include "ControllerBlock.h"

void euler_step
(
	struct StrictlyProperBlockInfo const * const bi,
	OutputFunction const h,
	PhysicsFunction const f,
	double * const nextState, // (1 x numStates)
	double * const currentdState, // (1 x numStates)
	double * const currentOutput, // (1 x numOutputs)
	double const dt,
	double const currentTime,
	double const * const currentState, // (1 x numStates)
	double const * const currentInput // (1 x numInputs)
);

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