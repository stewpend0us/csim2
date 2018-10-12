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
	FLOAT_TYPE dt = 0.1;
	FLOAT_TYPE time = 0.0;
	FLOAT_TYPE state[count] = {0};
	FLOAT_TYPE input[count];
	for (int i = 0; i<count; i++)
		input[i] = i+1;
	euler( &block, nextState, dState, dt, time, state, input );
	for (int i = 0; i<count; i++)
	{
		ASSERT( nextState[i] == dt*input[i], "iteration %d: %f should equal %f", i, nextState[i], dt*input[i]);
		ASSERT( dState[i] == input[i], "iteration %d: %f should equal %f", i, dState[i], input[i] );
	}
	PASS;
}
