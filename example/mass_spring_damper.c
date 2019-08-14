#include "block.h"
#include "solver.h"
#include "ascii_plot.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

// define our storage struct
struct mass_storage
{
	FLOAT_TYPE mass;
	FLOAT_TYPE stiffness;
	FLOAT_TYPE damping;
};

// define our physics function
static void physics(
	FLOAT_TYPE time,
	size_t num_states,
	FLOAT_TYPE dState[],
	FLOAT_TYPE const state[],
	size_t num_inputs,
	FLOAT_TYPE input[],
	void * storage
)
{
	(void)time;
	(void)num_inputs;
	(void)num_states;

	// decompose state
	FLOAT_TYPE xdot = state[0];
	FLOAT_TYPE x = state[1];

	// decompose dState
	FLOAT_TYPE * dxdot = &dState[0];
	FLOAT_TYPE * dx = &dState[1];

	// decompose input
	FLOAT_TYPE Force = input[0];

	// decompose storage
	struct mass_storage * mass_storage = storage;
	FLOAT_TYPE mass = mass_storage->mass;
	FLOAT_TYPE stiffness = mass_storage->stiffness;
	FLOAT_TYPE damping = mass_storage->damping;

	// calculate dState
	//mass*xdotdot + damping*xdot + stiffness*x = Force
	//mass*xdotdot = Force - damping*xdot - stiffness*x
	*dxdot = (Force - damping*xdot - stiffness*x)/mass;
	*dx = xdot;
}

int main( void )
{
	// block properties
	struct mass_storage storage = {10, 100, 15};

	//solver properties
	FLOAT_TYPE dt = .1;
	char solver = 'e'; // e for euler r for rk4
	
	// initialize our block
	struct block block = {2, 1, &storage, physics};

	// allocate memory for the solvers
	FLOAT_TYPE state[2] = {0, 1}; // initial velocity of 0, initial position of 1
	FLOAT_TYPE next_state[2];
	FLOAT_TYPE dstate[2];
	FLOAT_TYPE dB[2];
	FLOAT_TYPE dC[2];
	FLOAT_TYPE dD[2];
	FLOAT_TYPE time = 0;
	FLOAT_TYPE input[1] = {0};

	// solver loop
	for(;;)
	{
		ascii_plot( time, state[1], state[0] );

		if (solver == 'e')
			euler( &block, next_state, dstate, dt, &time, state, input );
		else if (solver == 'r')
			rk4( &block, next_state, dstate, dB, dC, dD, dt, &time, state, input, input, input );
		else
		{
			printf("bad solver %c\n", solver);
			return 1;
		}

		memcpy(state, next_state, 2*sizeof(FLOAT_TYPE));
		usleep(1000*1000*dt);
	}
}
