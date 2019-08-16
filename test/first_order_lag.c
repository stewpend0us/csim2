#include "block_basic.h"
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

	ASSERT( !first_order_lag( NULL, count, tau ), "should return NULL" );
	ASSERT( !first_order_lag( &block, 0, tau ), "should return NULL");
	ASSERT( !first_order_lag( &block, count, NULL ), "should return NULL");
	ASSERT( !first_order_lag( &block, count, tau ), "should return NULL");
	for (size_t i = 0; i<count; i++)
		tau[i] = i+1;
	ASSERT( first_order_lag( &block, count, tau ) == &block, "should return the pointer we passed in");
	ASSERT( block.num_states == count, "should be %d", count);
	ASSERT( block.num_inputs == count, "should be %d", count);
	ASSERT( block.storage == tau, "should be the array we passed in");
	ASSERT( block.f, "should not be NULL");
	block.f( time, block.num_states, dState, state, block.num_inputs, input, block.storage );
	ASSERT( memcmp( dState, input, count * sizeof( FLOAT_TYPE ) ) == 0, "should return zero dState" );
	for (size_t i = 0; i<count; i++)
		input[i] = i;
	block.f( time, block.num_states, dState, state, block.num_inputs, input, block.storage );
	for (size_t i = 0; i<count; i++)
		ASSERT( dState[i] <= input[i], "dState has to be less than or equal to the input" );
	PASS;
}
