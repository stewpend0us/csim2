#include "block_solver.h"
#include "block_basic.h"
#include "test.h"

int main(void)
{
	struct block block;
	#define count 500
	ASSERT( integrator( &block, count ) == &block, "should return the pointer we passed in");
	FLOAT_TYPE nextstate[count];
	FLOAT_TYPE dState[count];
	FLOAT_TYPE dt = 0.1;
	FLOAT_TYPE time = 0.0;
	FLOAT_TYPE state[count] = {0};
	FLOAT_TYPE input[count];
	for (int i = 0; i<count; i++)
		input[i] = i+1;
	euler( &block, nextstate, dState, dt, &time, state, input );
	for (int i = 0; i<count; i++)
	{
		ASSERT( dState[i] == input[i], "iteration %d: %f should equal %f", i, dState[i], input[i] );
		ASSERT( nextstate[i] == dt*input[i], "iteration %d: %f should equal %f error %g", i, nextstate[i], dt*input[i], nextstate[i] - dt*input[i]);
	}
	PASS;
}
