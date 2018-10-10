#include "integrator.h"
#include "test.h"
#include <string.h>

int main(void)
{
	struct block block;
	#define count 3
	FLOAT_TYPE dState[count] = {0};
	FLOAT_TYPE state[count] = {0};
	FLOAT_TYPE input[count] = {0};
	FLOAT_TYPE time = 10.0;

	ASSERT( !integrator( NULL, count ), "should return NULL" );
	ASSERT( !integrator( &block, 0 ), "should return NULL");
	ASSERT( integrator( &block, count ) == &block, "should return the pointer we passed in");
	ASSERT( block.numStates == count, "should be %d", count);
	ASSERT( block.numInputs == count, "should be %d", count);
	ASSERT( !block.storage, "should be NULL");
	ASSERT( block.f, "should not be NULL");
	block.f( &block, dState, time, state, input );
	ASSERT( memcmp( dState, input, count * sizeof( FLOAT_TYPE ) ) == 0, "should just move input to dState" );
	for (int i = 0; i<count; i++)
		input[i] = i;
	block.f( &block, dState, time, state, input );
	ASSERT( memcmp( dState, input, count * sizeof( FLOAT_TYPE ) ) == 0, "should just move input to dState" );
	PASS;
}
