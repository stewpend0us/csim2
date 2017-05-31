#pragma once
#include "StrictlyProperBlock.h"

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