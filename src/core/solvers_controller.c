#include <string.h>
#include "solvers_controller.h"

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
	double * const U, // numSteps x numInputs input values over time
	size_t numOutputs,
	double * const Y
)
{
	double * const temp_memory = malloc(block.numStates * 2 * sizeof(double));
	if (!temp_memory)
		return;

	double * const currentState = &temp_memory[0 * block.numStates];
	double * const nextState = currentState; //in this case it's ok that these are the same block of memory
	double * const currentdState = &temp_memory[1 * block.numStates];

	double * currentInput;
	double const * currentCommand;
	double * currentOutput;

	memcpy(currentState, Xi, block.numStates * sizeof(double));
	size_t i;
	for (i = 0; i < (numSteps - 1); i++)
	{
		currentInput = &U[i*block.numInputs];
		currentCommand = &C[i*numCommands];
		currentOutput = &Y[i*block.numOutputs];

		control(block.numInputs, block.numOutputs, numCommands, time[i], currentInput, currentOutput, currentCommand, controlStorage);
		block.h(block.numStates, block.numOutputs, currentOutput, time[i], currentState, block.storage);
		euler_f_step(block.numStates, block.numInputs, dt, time[i], nextState, currentdState, currentState, currentInput, block.f, block.storage);
	}

	//currentState = nextState;
	currentInput = &U[i*block.numInputs];
	currentCommand = &C[i*numCommands];
	currentOutput = &Y[i*block.numOutputs];

	control(block.numInputs, block.numOutputs, numCommands, time[i], currentInput, currentOutput, currentCommand, controlStorage);
	block.h(block.numStates, block.numOutputs, currentOutput, time[i], currentState, block.storage);

	free(temp_memory);
}

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
	double const * const Uc, // numSteps x numCommands inputs
	size_t numInputs,
	double * const U, // numSteps x numInputs input values over time
	size_t numOutputs,
	double * const Y
)
{
	double * const temp_memory = malloc(block.numStates * 6 * sizeof(double));
	if (!temp_memory)
		return;

	double * const currentState = &temp_memory[0 * block.numStates];
	double * const nextState = &temp_memory[1 * block.numStates];
	double * const currentdState = &temp_memory[2 * block.numStates];
	double * const B = &temp_memory[3 * block.numStates];
	double * const C = &temp_memory[4 * block.numStates];
	double * const D = &temp_memory[5 * block.numStates];

	double * currentInput;
	double const * currentCommand;
	double * currentOutput;

	memcpy(currentState, Xi, block.numStates * sizeof(double));
	size_t i;
	for (i = 0; i < (numSteps - 1); i++)
	{
		currentInput = &U[i*block.numInputs];
		currentCommand = &Uc[i*numCommands];
		currentOutput = &Y[i*block.numOutputs];

		control(block.numInputs, block.numOutputs, numCommands, time[i], currentInput, currentOutput, currentCommand, controlStorage);
		block.h(block.numStates, block.numOutputs, currentOutput, time[i], currentState, block.storage);
		rk4_f_step(block.numStates, block.numInputs, dt, time[i], nextState, currentdState, B, C, D, currentState, currentInput, currentInput, currentInput, block.f, block.storage);
		memcpy(currentState, nextState, block.numStates * sizeof(double));
	}
	currentInput = &U[i*block.numInputs];
	currentCommand = &Uc[i*numCommands];
	currentOutput = &Y[i*block.numOutputs];

	control(block.numInputs, block.numOutputs, numCommands, time[i], currentInput, currentOutput, currentCommand, controlStorage);
	block.h(block.numStates, block.numOutputs, currentOutput, time[i], currentState, block.storage);

	free(temp_memory);
}
