#pragma once
#include <stdlib.h>

struct StrictlyProperBlock
{
	size_t const numStates;
	size_t const numInputs;
	size_t const numOutputs;
	void * storage;
	void(*f)(double * const dState, double t, double const * const state, double const * const input, size_t const numStates, size_t const numInputs,  void * const storage);
	void(*h)(double * const output, double t, double const * const state, size_t numStates, size_t const numOutputs, void * const storage);
};

struct StrictlyProperBlock integrator(size_t numSignals);
struct StrictlyProperBlock system_new(size_t numInputs, size_t numOutputs, struct StrictlyProperBlock * blocks, void(*updateInputs)(double ** inputs, double const ** const outputs));
void system_free(struct StrictlyProperBlock * system);