#pragma once
#include <stdlib.h>

// function pointer types:

// update dState as a function of time, state, and input
typedef void(*PhysicsFunction)(
	size_t const numDstates,
	double * const dState,
	double const time,
	size_t const numStates,
	double const * const state,
	size_t const numInputs,
	double const * const input,
	void * const storage
	);

// update output as a function of time, and state
typedef void(*OutputFunction)(
	size_t const numOutputs,
	double * const output,
	double const time,
	size_t const numStates,
	double const * const state,
	void * const storage
	);

// data types:

// StrictlyProperBlock data
struct StrictlyProperBlock
{
	size_t const numStates;
	size_t const numInputs;
	size_t const numOutputs;
	PhysicsFunction const f;
	OutputFunction const h;
	void * const storage;
};

// solvers:

// euler solver
void euler_f_step
(
	double const ti, // current time
	double const dt, // time step
	size_t const xi,
	double * const Xt1, // (1 * xi) next State
	double * const A, // (1 * xi) current dstate
	double const * const Xti, // (1 * xi) current state/initial conditions
	size_t const ui,
	double const * const Uti, // (1 * ui) input at time ti
	PhysicsFunction const f,
	void * const storage
);

// rk4 solver
void rk4_f_step
(
	double const ti, // current time
	double const dt, // time step
	size_t const xi,
	double * const Xt1, // (1 * xi) next State
	double * const A, // (1 * xi) current dstate
	double * const B, // (1 * xi) solver storage/temp
	double * const C, // (1 * xi) solver storage/temp
	double * const D, // (1 * xi) solver storage/temp
	double * const Xtmp, // (1 * xi) solver storage/temp
	double const * const Xti, // (1 * xi) current state/initial conditions
	size_t const ui,
	double const * const Uti, // (1 * ui) input at time ti
	double const * const Ut2, // (1 * ui) input at time ti + dt/2
	double const * const Ut1, // (1 * ui) input at time ti + dt
	PhysicsFunction const f,
	void * const storage
);

// other:

size_t numSteps
(
	double const ti,
	double const dt,
	double const tf
);