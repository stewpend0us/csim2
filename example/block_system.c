#include "block.h"
#include "blockSystem.h"
#include "firstOrderLag.h"
#include "secondOrderSystem.h"
#include "solver.h"
#include "ascii_plot.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void updateCInputs(
	FLOAT_TYPE time,
	size_t num_children,
	struct block * child,
	FLOAT_TYPE * const child_input[],
	FLOAT_TYPE const * const child_state[],
	size_t num_inputs,
	FLOAT_TYPE const system_input[],
	void * storage
	)
{
	(void)time;
	(void)num_children;
	(void)child;
	(void)num_inputs;

	FLOAT_TYPE scale = *(FLOAT_TYPE*)storage;
	child_input[0][0] = scale*system_input[0];
	child_input[1][0] = child_state[0][0];
}

#define CHILD_COUNT 2
#define NUM_STATE 3	
int main( void )
{
	// block properties
	FLOAT_TYPE tau = 1.0;
	struct secondOrderSystemStorage storage = { 0.2, 1.0, 1.0};
	FLOAT_TYPE scale = 0.5;

	//solver properties
	FLOAT_TYPE dt = .1;
	char solver = 'e'; // e for euler r for rk4

	//Initialize the struct blockSystem
	struct block children[CHILD_COUNT];
	FLOAT_TYPE * cstate[CHILD_COUNT];
	FLOAT_TYPE * cinput[CHILD_COUNT];
	struct blockSystem system = { CHILD_COUNT, 1, children, cstate, cinput, &scale, updateCInputs };

	// initialize the child blocks
	firstOrderLag( &children[0], 1, &tau );
	secondOrderSystem( &children[1], 1, &storage );

	// initialize the system block
	struct block block;
	blockSystem( &block, &system );

	// allocate memory for the solvers
	FLOAT_TYPE state[NUM_STATE] = { 0.0 };
	FLOAT_TYPE next_state[NUM_STATE];
	FLOAT_TYPE dstate[NUM_STATE];
	FLOAT_TYPE dB[NUM_STATE];
	FLOAT_TYPE dC[NUM_STATE];
	FLOAT_TYPE dD[NUM_STATE];
	FLOAT_TYPE time = 0;
	FLOAT_TYPE input[3] = {1,0,0}; // this must be large enough for ALL inputs {system, block1, block2}

	// the solver loop
	for(;;)
	{
		ascii_plot( time, state[1], state[2] );

		if (solver == 'e')
			euler( &block, next_state, dstate, dt, &time, state, input );
		else if (solver == 'r')
			rk4( &block, next_state, dstate, dB, dC, dD, dt, &time, state, input, input, input );
		else
		{
			printf("bad solver %c\n", solver);
			return 1;
		}

		memcpy(state, next_state, NUM_STATE*sizeof(FLOAT_TYPE));
		usleep(1000*1000*dt);
	}
}
