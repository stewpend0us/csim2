#include "solver.h"
#include "integrator.h"
#include "test.h"

int main(void)
{
	struct block block;
	#define count 500
	ASSERT( integrator( &block, count ) == &block, "should return the pointer we passed in");
	FLOAT_TYPE nextState[count];
	FLOAT_TYPE dState[count];
	FLOAT_TYPE dB[count];
	FLOAT_TYPE dC[count];
	FLOAT_TYPE dD[count];
	FLOAT_TYPE dt = 0.1;
	FLOAT_TYPE time = 0.0;
	FLOAT_TYPE state[count] = {0};
	FLOAT_TYPE input[count];
	FLOAT_TYPE inputHalf[count];
	for (int i = 0; i<count; i++)
	{
		input[i] = i+1;
		inputHalf[i] = input[i];
	}
	rk4( &block, nextState, dState, dB, dC, dD, dt, &time, state, input, inputHalf, input );
	for (int i = 0; i<count; i++)
	{
		ASSERT( dState[i] == input[i], "iteration %d: %f should equal %f", i, dState[i], input[i] );
		ASSERT( dB[i] == inputHalf[i], "iteration %d: %f should equal %f", i, dB[i], inputHalf[i] );
		ASSERT( dC[i] == inputHalf[i], "iteration %d: %f should equal %f", i, dC[i], inputHalf[i] );
		ASSERT( dD[i] == inputHalf[i], "iteration %d: %f should equal %f", i, dD[i], input[i] );
		ASSERT( nextState[i] == dt*(input[i] + 2*inputHalf[i])/3, "iteration %d: %f should equal %f error %g", i, nextState[i], dt*(input[i] + 2*inputHalf[i])/3, nextState[i] - dt*(input[i] + 2*inputHalf[i])/3);
	}
	PASS;
}
