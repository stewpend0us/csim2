#pragma once
#include "StrictlyProperBlock.h"

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