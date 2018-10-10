#include "firstOrderLag.h"
#include "test.h"
#include <string.h>

int main(void)
{
	struct block block;
	#define count 3
	FLOAT_TYPE dState[count] = {0};
	FLOAT_TYPE state[count] = {0};
	FLOAT_TYPE input[count] = {0};
	FLOAT_TYPE tau[count] = {0};
	FLOAT_TYPE time = 10.0;

	ASSERT( !firstOrderLag( NULL, count, tau ), "should return NULL" );
	ASSERT( !firstOrderLag( &block, 0, tau ), "should return NULL");
	ASSERT( !firstOrderLag( &block, count, NULL ), "should return NULL");
	ASSERT( !firstOrderLag( &block, count, tau ), "should return NULL");
	for (int i = 0; i<count; i++)
		tau[i] = i+1;
	ASSERT( firstOrderLag( &block, count, tau ) == &block, "should return the pointer we passed in");
	ASSERT( block.numStates == count, "should be %d", count);
	ASSERT( block.numInputs == count, "should be %d", count);
	ASSERT( block.storage == tau, "should be the array we passed in");
	ASSERT( block.f, "should not be NULL");
	block.f( &block, dState, time, state, input );
	ASSERT( memcmp( dState, input, count * sizeof( FLOAT_TYPE ) ) == 0, "should return zero dState" );
	for (int i = 0; i<count; i++)
		input[i] = i;
	block.f( &block, dState, time, state, input );
	for (int i = 0; i<count; i++)
		ASSERT( dState[i] <= input[i], "dState has to be less than or equal to the input" );
	PASS;
}
