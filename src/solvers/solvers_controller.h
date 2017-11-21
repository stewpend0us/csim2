#pragma once
#include "solvers.h"



void euler_c
(
	struct StrictlyProperBlock const * const block,
	struct ControllerBlock const * const controller,
	double * const C, // numSteps x numCommands (controller)
	double * const Y, // numSteps x numOutputs (block)
	double const dt, //time step
	size_t numSteps, //size of time vector
	double const * const time, //numSteps x 1 time vector
	double const * const Xi, // 1 x numStates initial conditions vector
	double const * const U // numSteps x numInputs (controller)
);

void rk4_c
(
	struct StrictlyProperBlock const * const block,
	struct ControllerBlock const * const controller,
	double * const C, // numSteps x numCommands (controller)
	double * const Y, // numSteps x numOutputs (block)
	double const dt, //time step
	size_t numSteps, //size of time vector
	double const * const time, //numSteps x 1 time vector
	double const * const Xi, // 1 x numStates initial conditions vector
	double const * const U // numSteps x numInputs (controller)
);