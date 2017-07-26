#pragma once
#include "solvers.h"

typedef void(*ControllerFunction)(
	size_t const numOutput,
	size_t const numFeedback,
	size_t const numCommand,
	double const time,
	double * const output,
	double const * const feedback,
	double const * const command,
	void * const storage
	);

void euler_c
(
	struct StrictlyProperBlock block,
	ControllerFunction control,
	void * const controlStorage,
	double const dt, //time step
	size_t numSteps, //size of time vector
	double const * const time, //numSteps x 1 time vector
	size_t numStates,
	double const * const Xi, // 1 x numStates initial conditions vector
	size_t numCommands,
	double const * const C, // numSteps x numCommand inputs
	size_t numInputs,
	double const * const U, // numSteps x numInputs input values over time
	size_t numOutputs,
	double * const Y
);

void rk4_c
(
	struct StrictlyProperBlock block,
	ControllerFunction control,
	void * const controlStorage,
	double const dt, //time step
	size_t numSteps, //size of time vector
	double const * const time, //time vector
	size_t numStates,
	double const * const Xi, // numStates x 1 initial conditions vector
	size_t numCommands,
	double const * const C, // numSteps x numCommands inputs
	size_t numInputs,
	double const * const U, // numSteps x numInputs input values over time
	size_t numOutputs,
	double * const Y
);