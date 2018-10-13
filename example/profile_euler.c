#include "solver.h"
#include "firstOrderLag.h"
#include <stdio.h>

#define str(x) #x

int main(void)
{
	struct block block;
	#define count 5000
	FLOAT_TYPE tau[count];
	//FLOAT_TYPE nextState[count];
	FLOAT_TYPE dState[count];
	FLOAT_TYPE dt = 0.01;
	FLOAT_TYPE time = 0.0;
	FLOAT_TYPE state[count] = {0};
	FLOAT_TYPE input[count];
	#define path ("euler_profile_t_" str(count) "_" str(FLOAT_TYPE) ".profile")
	remove(path);
	FILE * F = fopen(path, "ab");

	for (size_t i = 0; i<count; i++)
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
		euler( &block, state, dState, dt, time, state, input );
		time += dt;
		fwrite(&time, sizeof(FLOAT_TYPE), 1, F);
		fwrite(state, sizeof(FLOAT_TYPE)*count, 1, F);
	}
	fclose(F);
}
