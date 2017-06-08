#pragma once
#include "StrictlyProperBlock.h"

// euler solver
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

// rk4 solver
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

// other:

//size_t numSteps
//(
//	double const ti,
//	double const dt,
//	double const tf
//);