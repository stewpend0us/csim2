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

// euler solver
void euler
();

// rk4 solver
void rk4
(
	double * const Y,
	double * const Xi,
	struct StrictlyProperBlock const * const block,
	size_t const num_t,
	double const * const t,
	double const * const U
);