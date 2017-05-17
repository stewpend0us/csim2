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

struct SystemStorage
{
	size_t numBlocks;
	struct StrictlyProperBlock const * const blocks;
	double * const * const blockInputs;
	double * const * const blockOutputs;
	void(*updateInputs)(double * const * const inputs, double const * const * const outputs);
};

struct SystemStorage system_storage_new(size_t numBlocks, struct StrictlyProperBlock const * const blocks, void(*updateInputs)(double * const * const inputs, double const * const * const outputs));
void system_storage_free(struct SystemStorage const * const storage);

struct StrictlyProperBlock blockSystem(size_t numInputs, size_t numOutputs, struct SystemStorage * const storage);
