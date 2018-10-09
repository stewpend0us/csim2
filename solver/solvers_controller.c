#include <string.h>
#include "dbg.h"
#include "solvers_controller.h"

void euler_c
(
	struct StrictlyProperBlock const * const block,
	struct ControllerBlock const * const controller,
	double * const C, // numSteps x numCommands (controller)
	double * const Y, // numSteps x numOutputs (block)
	double const dt, //time step
	size_t numSteps, //size of time vector
	double const * const time, //numSteps x 1 time vector
	double const * const Xi, // 1 x numStates initial conditions vector
	double const * const U // numSteps x numInputs (controller)
)
{
	size_t const numStates = block->numStates;
	OutputFunction const h = block->h;

	ControllerFunction const c = controller->c;

	double * const temp_memory = malloc(numStates * 2 * sizeof(double));
	check_mem(temp_memory);

	double * const currentState = &temp_memory[0 * numStates];
	double * const nextState = currentState; //in this case it's ok that these are the same block of memory
	double * const currentdState = &temp_memory[1 * numStates];

	double * ctrlFeedback;
	double const * ctrlInput;
	double * ctrlCommand;

	memcpy(currentState, Xi, numStates * sizeof(double));
	size_t i;
	for (i = 0; i < (numSteps - 1); i++)
	{
		ctrlFeedback = &Y[i*controller->numCommands];
		ctrlInput = &U[i*controller->numInputs];
		ctrlCommand = &C[i*controller->numFeedback];

		c(controller, ctrlCommand, time[i], ctrlFeedback, ctrlInput);
		euler_step(block, nextState, currentdState, ctrlFeedback, dt, time[i], currentState, ctrlCommand);
	}

	//currentState = nextState;
	ctrlFeedback = &Y[i*controller->numCommands];
	ctrlInput = &U[i*controller->numInputs];
	ctrlCommand = &C[i*controller->numFeedback];

	c(controller, ctrlCommand, time[i], ctrlFeedback, ctrlInput);
	h(block, ctrlCommand, time[i], currentState);

error:
	free(temp_memory);
}

void rk4_c
(
	struct StrictlyProperBlock const * const block,
	struct ControllerBlock const * const controller,
	double * const uC, // numSteps x numCommands (controller)
	double * const Y,
	double const dt, //time step
	size_t numSteps, //size of time vector
	double const * const time, //time vector
	double const * const Xi, // numStates x 1 initial conditions vector
	double const * const U // numSteps x numInputs (controller)
)
{
	size_t const numStates = block->numStates;
	OutputFunction const h = block->h;

	ControllerFunction const c = controller->c;

	double * const temp_memory = malloc(numStates * 6 * sizeof(double));
	check_mem(temp_memory);

	double * const currentState = &temp_memory[0 * numStates];
	double * const nextState = &temp_memory[1 * numStates];
	double * const currentdState = &temp_memory[2 * numStates];
	double * const B = &temp_memory[3 * numStates];
	double * const C = &temp_memory[4 * numStates];
	double * const D = &temp_memory[5 * numStates];

	double * ctrlFeedback;
	double const * ctrlInput;
	double * ctrlCommand;

	memcpy(currentState, Xi, numStates * sizeof(double));
	size_t i;
	for (i = 0; i < (numSteps - 1); i++)
	{
		ctrlFeedback = &Y[i*controller->numFeedback];
		ctrlInput = &U[i*controller->numInputs];
		ctrlCommand = &uC[i*controller->numCommands];
		
		c(controller, ctrlCommand, time[i], ctrlFeedback, ctrlInput);
		rk4_step(block,nextState,currentdState,B,C,D,ctrlFeedback,dt,time[i],currentState,ctrlCommand,ctrlCommand,ctrlCommand);
		memcpy(currentState, nextState, numStates * sizeof(double));
	}
	ctrlFeedback = &Y[i*controller->numFeedback];
	ctrlInput = &U[i*controller->numCommands];
	ctrlCommand = &uC[i*controller->numCommands];

	c(controller, ctrlCommand, time[i], ctrlFeedback, ctrlInput);
	h(block, ctrlFeedback, time[i], currentState);

error:
	free(temp_memory);
}
