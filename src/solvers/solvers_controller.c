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
	struct StrictlyProperBlockInfo const bi = block->info;
	PhysicsFunction const f = block->f;
	OutputFunction const h = block->h;
	UtilityFunction const u = block->u;

	struct ControllerBlockInfo const ci = controller->info;
	ControllerFunction const c = controller->c;

	double * const temp_memory = malloc(bi.numStates * 2 * sizeof(double));
	check_mem(temp_memory);

	double * const currentState = &temp_memory[0 * bi.numStates];
	double * const nextState = currentState; //in this case it's ok that these are the same block of memory
	double * const currentdState = &temp_memory[1 *bi.numStates];

	double * ctrlFeedback;
	double const * ctrlInput;
	double * ctrlCommand;

	memcpy(currentState, Xi, bi.numStates * sizeof(double));
	size_t i;
	for (i = 0; i < (numSteps - 1); i++)
	{
		ctrlFeedback = &Y[i*ci.numCommands];
		ctrlInput = &U[i*ci.numInputs];
		ctrlCommand = &C[i*ci.numFeedback];

		c(&ci, ctrlCommand, time[i], ctrlFeedback, ctrlInput);
		//h(&bi, ctrlFeedback, time[i], currentState);
		euler_step(&bi, h, f, u, nextState, currentdState, ctrlFeedback, dt, time[i], currentState, ctrlCommand);
	}

	//currentState = nextState;
	ctrlFeedback = &Y[i*ci.numCommands];
	ctrlInput = &U[i*ci.numInputs];
	ctrlCommand = &C[i*ci.numFeedback];

	c(&ci, ctrlCommand, time[i], ctrlFeedback, ctrlInput);
	h(&bi, ctrlCommand, time[i], currentState);

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
	struct StrictlyProperBlockInfo const bi = block->info;
	PhysicsFunction const f = block->f;
	OutputFunction const h = block->h;
	UtilityFunction const u = block->u;

	struct ControllerBlockInfo const ci = controller->info;
	ControllerFunction const c = controller->c;

	double * const temp_memory = malloc(bi.numStates * 6 * sizeof(double));
	check_mem(temp_memory);

	double * const currentState = &temp_memory[0 * bi.numStates];
	double * const nextState = &temp_memory[1 * bi.numStates];
	double * const currentdState = &temp_memory[2 * bi.numStates];
	double * const B = &temp_memory[3 * bi.numStates];
	double * const C = &temp_memory[4 * bi.numStates];
	double * const D = &temp_memory[5 * bi.numStates];

	double * ctrlFeedback;
	double const * ctrlInput;
	double * ctrlCommand;

	memcpy(currentState, Xi, bi.numStates * sizeof(double));
	size_t i;
	for (i = 0; i < (numSteps - 1); i++)
	{
		ctrlFeedback = &Y[i*ci.numFeedback];
		ctrlInput = &U[i*ci.numInputs];
		ctrlCommand = &uC[i*ci.numCommands];
		
		c(&ci, ctrlCommand, time[i], ctrlFeedback, ctrlInput);
		rk4_step(&bi,h,f,u,nextState,currentdState,B,C,D,ctrlFeedback,dt,time[i],currentState,ctrlCommand,ctrlCommand,ctrlCommand);
		memcpy(currentState, nextState, bi.numStates * sizeof(double));
	}
	ctrlFeedback = &Y[i*ci.numFeedback];
	ctrlInput = &U[i*ci.numCommands];
	ctrlCommand = &uC[i*ci.numCommands];

	c(&ci, ctrlCommand, time[i], ctrlFeedback, ctrlInput);
	h(&bi, ctrlFeedback, time[i], currentState);

error:
	free(temp_memory);
}
