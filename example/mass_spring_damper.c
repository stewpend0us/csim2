#include "block.h"
#include "solver.h"
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

struct mass_storage
{
	FLOAT_TYPE mass;
	FLOAT_TYPE stiffness;
	FLOAT_TYPE damping;
};

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

static void ascii_plot(
	FLOAT_TYPE const * state
)
{
	FLOAT_TYPE velocity = state[0];
	FLOAT_TYPE position = state[1];
	FLOAT_TYPE thresh = .1;
	char row[] = ":                                                                                  :";
	int space = sizeof(row)/sizeof(row[0]) - 1;
	int pos = round((position*space + space)/2) + 1;
	if ( 0 < pos && pos < space-1 )
	{
		if (velocity < -thresh) row[pos] = '/';
		else if (velocity > thresh) row[pos] = '\\';
		else row[pos] = '|';
	}
	printf("%s\n",row);
}

int main(int argc, char **argv)
{
	struct mass_storage storage = {10, 100, 15};
	FLOAT_TYPE dt = .1;
	FLOAT_TYPE tf = 10;
	char solver = 'e';
	switch (argc)
	{
		case 7: solver = argv[6][0];
		case 6: tf = atof(argv[5]);
		case 5: dt = atof(argv[4]);
		case 4: storage.damping = atof(argv[3]);
		case 3: storage.stiffness = atof(argv[2]);
		case 2: storage.mass = atof(argv[1]);
	}
	printf(
		"mass:      %f\n"
		"stiffness: %f\n"
		"damping:   %f\n"
		"dt:        %f\n"
		"tf:        %f\n", storage.mass, storage.stiffness, storage.damping, dt, tf);
	sleep(1);
	
	struct block block = {2, 1, &storage, physics};
	FLOAT_TYPE state[2] = {0, 1}; // initial velocity of 0, initial position of 1
	FLOAT_TYPE next_state[2];
	FLOAT_TYPE dstate[2];
	FLOAT_TYPE dB[2];
	FLOAT_TYPE dC[2];
	FLOAT_TYPE dD[2];
	FLOAT_TYPE time = 0;
	FLOAT_TYPE input[1] = {0};

	if (solver != 'e' && solver != 'r')
	{
		printf("No %c solver\n",solver);
		return 1;
	}

	while ( time < tf )
	{
		ascii_plot( state );
//		printf("%5.2f %5.2f\n",state[0],state[1]);

		if (solver == 'e')
			euler( &block, next_state, dstate, dt, &time, state, input );
		if (solver == 'r')
			rk4( &block, next_state, dstate, dB, dC, dD, dt, &time, state, input, input, input );

		memcpy(state, next_state, 2*sizeof(FLOAT_TYPE));
		usleep(1000*1000*dt);
	}
}
