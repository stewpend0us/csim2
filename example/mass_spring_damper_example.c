#include "multistep.h"
#include "dbg.h"
#include <math.h>
#include <unistd.h>
#include <string.h>

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

/*
void delay(unsigned int mseconds)
{
    clock_t goal = mseconds + clock();
    while (goal > clock());
}
*/

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
	int const space = 20;
	int pos = round(position*space)+space;
	double const thresh = .1;
	printf(":");
	for (int i = 0; i < pos+1; i++)
		printf(" ");
	if (velocity < -thresh)
		printf("/");
	else if (velocity > thresh)
		printf("\\");
	else
		printf("|");
	for (int i = pos; i<2*space+1; i++)
		printf(" ");
	printf(":\n");
	usleep(1000*1000*storage->dt);
}

int main(int argc, char **argv)
{
	struct mass_storage storage = {10, 100, 10, .1};
	double tf = 10;
	char solver = 'e';
	switch (argc)
	{
		case 7:
			solver = argv[6][0];
		case 6:
			tf = atof(argv[5]);
		case 5:
			storage.dt = atof(argv[4]);
		case 4:
			storage.damping = atof(argv[3]);
		case 3:
			storage.stiffness = atof(argv[2]);
		case 2:
			storage.mass = atof(argv[1]);
		default:
			printf(
				"mass:      %f\n"
				"stiffness: %f\n"
				"damping:   %f\n"
				"dt:        %f\n"
				"tf:        %f\n", storage.mass, storage.stiffness, storage.damping, storage.dt, tf);
		sleep(1);
	}
	struct StrictlyProperBlock block = {2, 1, 2, &storage, outputState, physics, util};
	double const Xi[2] = {0, 1}; // initial velocity of 0, initial position of 1
	double * time = NULL;
	double * Y = NULL;
	double * U = NULL;

	size_t const numSteps = numTimeSteps(storage.dt, tf);

	time = malloc(numSteps * sizeof(double));
	check_mem(time);
	Y = malloc(2 * numSteps * sizeof(double));
	check_mem(Y);
	U = calloc(numSteps, sizeof(double));
	check_mem(U)

	initializeTime(numSteps, time, storage.dt, 0);
	switch (solver)
	{
		case 'e':
			euler(&block, Y, storage.dt, numSteps, time, Xi, U);
			break;
		case 'r':
			rk4(&block, Y, storage.dt, numSteps, time, Xi, U, U);
			break;
		default:
			printf("No %c solver\n",solver);
	}

error:
	free(time);
	free(Y);
	free(U);
}
