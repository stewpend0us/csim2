#include "block.h"
#include "block_solver.h"
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
	FLOAT_TYPE dstate[],
	FLOAT_TYPE const state[],
	size_t num_inputs,
	FLOAT_TYPE const input[],
	size_t num_outputs,
	FLOAT_TYPE output[],
	void * storage
)
{
	(void)time;
	(void)num_inputs;
	(void)num_states;
	(void)num_outputs;
	(void)output;

	// decompose state
	FLOAT_TYPE xdot = state[0];
	FLOAT_TYPE x = state[1];

	// decompose dState
	FLOAT_TYPE * dxdot = &dstate[0];
	FLOAT_TYPE * dx = &dstate[1];

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
	struct block const block = {2, 1, 0, &storage, physics};

	// allocate memory for the solvers
	FLOAT_TYPE state[block.num_states];
	FLOAT_TYPE next_state[block.num_states];
	FLOAT_TYPE dstate[block.num_states];
	FLOAT_TYPE dB[block.num_states];
	FLOAT_TYPE dC[block.num_states];
	FLOAT_TYPE dD[block.num_states];
	FLOAT_TYPE time = 0;
	FLOAT_TYPE input[block.num_inputs];

	// initial conditions
	state[0] = 0;
	state[1] = 1;
	input[0] = 1;

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
