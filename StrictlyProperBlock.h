#pragma once
#include <stdlib.h>

struct StrictlyProperBlock
{
	size_t const numStates;
	size_t const numInputs;
	size_t const numOutputs;
	void * storage;
	void(*f)(size_t const numStates, double * const dState, double t, double const * const state, size_t const numInputs, double const * const input, void * const storage);
	void(*h)(size_t const numOutputs, double * const output, double t, size_t numStates, double const * const state, void * const storage);
};

struct StrictlyProperBlock integrator(size_t numSignals);

typedef void(*UpdateBlockInputsFunction)(size_t numBlocks, struct StrictlyProperBlock const * const blocks, double * const * const inputs, double const * const * const outputs, size_t numSystemInputs, double const * const systemInput);
typedef void(*CalcSystemOutputFunction)(size_t numOutputs, double * const outputs, double const * const * const blockOutputs);

struct SystemStorage
{
	size_t numBlocks;
	struct StrictlyProperBlock const * const blocks;
	double * const * const blockInputs;
	double * const * const blockOutputs;
	UpdateBlockInputsFunction updateInputs;
	CalcSystemOutputFunction calcOutput;
};

struct SystemStorage system_storage_new(size_t numBlocks, struct StrictlyProperBlock const * const blocks, UpdateBlockInputsFunction updateInputs, CalcSystemOutputFunction calcOutputs);
void system_storage_free(struct SystemStorage const * const storage);

struct StrictlyProperBlock blockSystem(size_t numInputs, size_t numOutputs, struct SystemStorage * const storage);
