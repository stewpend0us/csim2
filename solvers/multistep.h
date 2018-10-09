#pragma once
#include "StrictlyProperBlock.h"
#include "singlestep.h"

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