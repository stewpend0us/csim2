#pragma once
#include <stdlib.h>

typedef void(*PhysicsFunction)(size_t const numDstates, double * const dState, double const t, size_t const numStates, double const * const state, size_t const numInputs, double const * const input, void * const storage);
typedef void(*OutputFunction)(size_t const numOutputs, double * const output, double const t, size_t const numStates, double const * const state, void * const storage);

struct StrictlyProperBlock
{
	size_t const numStates;
	size_t const numInputs;
	size_t const numOutputs;
	PhysicsFunction const f;
	OutputFunction const h;
	void * const storage;
};

struct StrictlyProperBlock integrator(size_t const numSignals);

typedef void(*UpdateBlockInputsFunction)(size_t const numBlocks, struct StrictlyProperBlock const * const blocks, double * const * const inputs, double const * const * const outputs, size_t const numSystemInputs, double const * const systemInput);
typedef void(*CalcSystemOutputFunction)(size_t const numOutputs, double * const outputs, double const * const * const blockOutputs);

struct SystemStorage
{
	size_t numBlocks;
	struct StrictlyProperBlock const * const blocks;
	double * const * const blockInputs;
	double * const * const blockOutputs;
	UpdateBlockInputsFunction const updateInputs;
	CalcSystemOutputFunction const calcOutput;
};

struct SystemStorage system_storage_new(size_t const numBlocks, struct StrictlyProperBlock const * const blocks, UpdateBlockInputsFunction const updateInputs, CalcSystemOutputFunction const calcOutputs);
void system_storage_free(struct SystemStorage const * const storage);

struct StrictlyProperBlock blockSystem(size_t const numInputs, size_t const numOutputs, struct SystemStorage * const storage);

void rk4(double * const Y, double * const Xi, struct StrictlyProperBlock const * const block, size_t const num_t, double const * const t, double const * const U);