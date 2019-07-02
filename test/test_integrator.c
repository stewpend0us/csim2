#include "integrator.h"
#include "test.h"
#include <string.h>

int main(void)
{
	struct block block;
	#define count 3
	FLOAT_TYPE dstate[count] = {0};
	FLOAT_TYPE state[count] = {0};
	FLOAT_TYPE input[count] = {0};
	FLOAT_TYPE time = 10.0;

	ASSERT( !integrator( NULL, count ), "should return NULL" );
	ASSERT( !integrator( &block, 0 ), "should return NULL");
	ASSERT( integrator( &block, count ) == &block, "should return the pointer we passed in");
	ASSERT( block.num_states == count, "should be %d", count);
	ASSERT( block.num_inputs == count, "should be %d", count);
	ASSERT( !block.storage, "should be NULL");
	ASSERT( block.f, "should not be NULL");
	block.f( time, block.num_states, dstate, state, block.num_inputs, input, block.storage );
	ASSERT( memcmp( dstate, input, count * sizeof( FLOAT_TYPE ) ) == 0, "should just move input to dstate" );
	for (int i = 0; i<count; i++)
		input[i] = i;
	block.f( time, block.num_states, dstate, state, block.num_inputs, input, block.storage );
	ASSERT( memcmp( dstate, input, count * sizeof( FLOAT_TYPE ) ) == 0, "should just move input to dstate" );
	PASS;
}
