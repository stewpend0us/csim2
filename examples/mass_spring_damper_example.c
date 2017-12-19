#include "multistep.h"
#include "dbg.h"
#include <math.h>
#include<time.h>

struct mass_storage
{
    double mass;
    double stiffness;
    double damping;
	double dt;
};

static void physics(
    struct StrictlyProperBlock const * const block,
	double * const dState,
	double const time,
	double const * const state,
	double const * const input
)
{
	(void) time;

	check_debug(block->numInputs == 1, "expecting 1 input but there were %zu", block->numInputs);
	check_debug(block->numStates == 2, "expecting 2 states but there were %zu", block->numStates);

	// decompose state
	double const xdot = state[0];
	double const x = state[1];

	// decompose dState
	double * const dxdot = &dState[0];
	double * const dx = &dState[1];

	// decompose input
	double const Force = input[0];

	// decompose storage
	struct mass_storage * const storage = block->storage;
	double const mass = storage->mass;
	double const stiffness = storage->stiffness;
	double const damping = storage->damping;
	
	// calculate dState
	//mass*xdotdot + damping*xdot + stiffness*x = Force
	//mass*xdotdot = Force - damping*xdot - stiffness*x
	*dxdot = (Force - damping*xdot - stiffness*x)/mass;
	*dx = xdot;
error:
	return;
}

void delay(unsigned int mseconds)
{
    clock_t goal = mseconds + clock();
    while (goal > clock());
}

static void util(
	struct StrictlyProperBlock const * const block,
	double const time,
	double const * const dState,
	double const * const state,
	double const * const input,
	double const * const output
)
{
	struct mass_storage * const storage = block->storage;;
	(void) time;
	(void) dState;
	(void) input;
	(void) output;
	double const velocity = state[0];
	double const position = state[1];
	int const space = 40;
	int const pos = floor(space*position)+space;
	double const thresh = .1;
	printf(":");
	for (int i = 0; i < pos; i++)
		printf(" ");
	if (velocity < -thresh)
		printf("/");
	else if (velocity > thresh)
		printf("\\");
	else
		printf("|");
	for (int i = pos; i<2*space; i++)
		printf(" ");
	printf(":\n");
	//return;
	//printf("%f\n",velocity);
	delay(storage->dt*1000);
}



int main()
{
	double const dt = 0.1;
	struct mass_storage storage = {100, 10, 2, dt};
	struct StrictlyProperBlock block = {2, 1, 2, &storage, outputState, physics, util};
	double const Xi[2] = {0, 1};
	double * time = NULL;
	double * Y = NULL;
	double * U = NULL;

	double const tf = 60;
	size_t const numSteps = numTimeSteps(dt, tf);
	
	time = malloc(numSteps * sizeof(double));
	check_mem(time);
	Y = malloc(2 * numSteps * sizeof(double));
	check_mem(Y);
	U = calloc(numSteps, sizeof(double));
	check_mem(U)

	initializeTime(numSteps, time, dt, 0);
	euler(&block, Y, dt, numSteps, time, Xi, U);

error:
	free(time);
	free(Y);
	free(U);
}