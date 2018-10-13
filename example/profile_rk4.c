#include "solver.h"
#include "firstOrderLag.h"
#include <stdio.h>
#include <string.h>

#define str(x) #x

int main(void)
{
	struct block block;
	#define count 5000
	FLOAT_TYPE tau[count];
	FLOAT_TYPE nextState[count];
	FLOAT_TYPE dState[count];
	FLOAT_TYPE dB[count];
	FLOAT_TYPE dC[count];
	FLOAT_TYPE dD[count];
	FLOAT_TYPE dt = 0.01;
	FLOAT_TYPE time = 0.0;
	FLOAT_TYPE state[count] = {0};
	FLOAT_TYPE input[count];
	#define path ("rk4_profile_t_" str(count) "_" str(FLOAT_TYPE) ".profile")
	remove(path);
	FILE * F = fopen(path, "ab");

	for (int i = 0; i<count; i++)
	{
		input[i] = 1.0;
		tau[i] = i+1;
	}
	if ( !firstOrderLag( &block, count, tau ) ) 
		return 1;
	fwrite(&time, sizeof(FLOAT_TYPE), 1, F);
	fwrite(state, sizeof(FLOAT_TYPE)*count, 1, F);
	while ( time < 5*60 )
	{
		rk4( &block, nextState, dState, dB, dC, dD, dt, time, state, input, input, input );
		time += dt;
		memcpy(state, nextState, sizeof(FLOAT_TYPE)*count);
		fwrite(&time, sizeof(FLOAT_TYPE), 1, F);
		fwrite(state, sizeof(FLOAT_TYPE)*count, 1, F);
	}
	fclose(F);
}
