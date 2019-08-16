#include "block_basic.h"
#include "block_system.h"
#include "test.h"
#include <string.h>

#define count 30

void update
(
	FLOAT_TYPE time,
	size_t num_children,
	struct block * child,
	FLOAT_TYPE * const child_input[],
	FLOAT_TYPE const * const child_state[],
	size_t num_inputs,
	FLOAT_TYPE const system_input[],
	void * storage
)
{
	ASSERT( time == 10.0, "should be %f", 10.0 );
	ASSERT( system_input, "should not be NULL" );
	ASSERT( num_children == count, "should be %d", count );
	ASSERT( num_inputs == 1, "should be 1" );
	ASSERT( child, "should not be NULL" );
	ASSERT( child_input, "should not be NULL" );
	ASSERT( !storage, "should be NULL" );
	for ( size_t i = 0; i<num_children; i++ )
	{
		ASSERT( child[i].num_states == 1, "expecting 1 state %zu", i );
		ASSERT( child[i].num_inputs == 1, "expecting 1 unput %zu", i );
		ASSERT( child_input[i], "should not be NULL" );
		ASSERT( child_state[i][0] == i, "expecting child state %zu to be initialized this way", i );
	}

	ASSERT(system_input[0] == 1, "expecting system input of 1");
	child_input[0][0] = system_input[0];
	ASSERT(child_input[0][0] == 1, "expecting system input of 1 to be assigned to child_input[0][0]");
	for ( size_t i = 1; i<num_children; i++ )
		child_input[i][0] = child_state[i-1][0];
}

int main(void)
{
	struct block block;
	struct block_system system = {0};
	struct block child[count] = {{0}};
	FLOAT_TYPE * child_state[count] = {0};
	FLOAT_TYPE * child_input[count] = {0};
	ASSERT( !block_system( NULL, &system ), "should return NULL" );
	ASSERT( !block_system( &block, NULL ), "should return NULL" );

	system.num_children = count;
	system.num_inputs = 1;
	system.child = child;
	system.child_state = child_state;
	system.child_input = child_input;
	system.storage = NULL;
	system.updateChildInputs = update;

	system.num_children = 0;
	ASSERT( !block_system( &block, &system ), "should return NULL. must have children" );

	system.num_children = count;
	system.child = NULL;
	ASSERT( !block_system( &block, &system ), "should return NULL. must allocate children" );

	system.child = child;
	system.child_state = NULL;
	ASSERT( !block_system( &block, &system ), "should return NULL. must have child state" );

	system.child_state = child_state;
	system.child_input = NULL;
	ASSERT( !block_system( &block, &system ), "should return NULL. must have child input" );

	system.child_input = child_input;
	system.updateChildInputs = NULL;
	ASSERT( !block_system( &block, &system ), "should return NULL. must have update function" );

	system.updateChildInputs = update;
	ASSERT( !block_system( &block, &system ), "should return NULL. children must be initialized" );

	for (size_t i = 0; i<count; i++)
	{
		ASSERT( integrator( &child[i], 1 ) == &child[i], "failed to create integrator %zu", i );
	}

	struct block * result = block_system( &block, &system );
	ASSERT( result == &block, "should return the pointer we passed in %p != %p", (void*)&block, (void*)result );
	ASSERT( block.storage == &system, "should be the pointer we passed in");
	ASSERT( block.num_states == count, "should be %d", count );
	ASSERT( block.num_inputs == system.num_inputs + count, "should be %zu", system.num_inputs + count );
	ASSERT( block.f, "should not be NULL" );

	FLOAT_TYPE dState[count] = {0};
	FLOAT_TYPE state[count];
	FLOAT_TYPE input[count+1] = {0};
	input[0] = 1;
	FLOAT_TYPE time = 10.0;

	for ( size_t i = 0; i<count; i++ )
		state[i] = i;

	ASSERT( block.num_inputs == count+1, "expecting the block input count to be count+1");
	block.f( time, block.num_states, dState, state, block.num_inputs, input, block.storage );
	ASSERT( dState[0] == input[0], "should just move input to dState" )
	for (size_t i = 1; i<count; i++)
	{
		ASSERT( dState[i] == state[i-1], "should just move input (state in this case) to dState %zu", i )
	}
	PASS;
}
