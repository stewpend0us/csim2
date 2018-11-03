#include "integrator.h"
#include "blockSystem.h"
#include "test.h"
#include <string.h>

#define count 3

void update
(
	struct blockSystem const * system,
	FLOAT_TYPE time,
	FLOAT_TYPE const systemInput[]
)
{
	ASSERT( system, "should not be NULL" );
	ASSERT( time == 10.0, "should be %f", 10.0 );
	ASSERT( systemInput, "should not be NULL" );
	ASSERT( system->childState, "should not be NULL" );
	ASSERT( system->numChildren == count, "should be %d", count );
	ASSERT( system->numInputs == 1, "should be 1" );
	ASSERT( system->child, "should not be NULL" );
	ASSERT( system->childInput, "should not be NULL" );
	ASSERT( !system->storage, "should be NULL" );
	ASSERT( system->updateChildInputs == update, "should be a pointer to this function");
	for ( size_t i = 0; i<count; i++ )
	{
		ASSERT( system->childState[i], "should not be NULL %zu", i );
		ASSERT( system->child[i].numStates == 1, "expecting 1 state %zu", i );
		ASSERT( system->child[i].numInputs == 1, "expecting 1 unput %zu", i );
		ASSERT( system->childInput[i], "should not be NULL" );
	}

	system->childInput[0][0] = systemInput[0];
	for ( size_t i = 1; i<count; i++ )
	{
		system->childInput[i][0] = system->childState[i-1][0];
	}
}

int main(void)
{
	struct block block;
	struct blockSystem system = {0};
	struct block child[count] = {{0}};
	FLOAT_TYPE * childInput[count] = {0};
	ASSERT( !blockSystem( NULL, &system ), "should return NULL" );
	ASSERT( !blockSystem( &block, NULL ), "should return NULL" );

	system.numInputs = 1;
	system.numChildren = 0;
	system.child = child;
	system.childInput = childInput;
	system.updateChildInputs = update;
	system.storage = NULL;
	ASSERT( !blockSystem( &block, &system ), "should return NULL" );

	system.numChildren = count;
	system.child = NULL;
	ASSERT( !blockSystem( &block, &system ), "should return NULL" );

	system.child = child;
	system.childInput = NULL;
	ASSERT( !blockSystem( &block, &system ), "should return NULL" );

	system.childInput = childInput;
	system.updateChildInputs = NULL;
	ASSERT( !blockSystem( &block, &system ), "should return NULL" );

	system.updateChildInputs = update;
	ASSERT( !blockSystem( &block, &system ), "should return NULL" );

	for (size_t i = 0; i<count; i++)
	{
		ASSERT( integrator( &child[i], 1 ) == &child[i], "failed to create integrator %zu", i );
	}
	ASSERT( !blockSystem( &block, &system ), "should return NULL" );
	for (size_t i = 0; i<count; i++)
	{
		childInput[i] = malloc( child[i].numInputs*sizeof(FLOAT_TYPE) );
		ASSERT( childInput[i], "malloc failed %zu", i );
	}
	ASSERT( blockSystem( &block, &system ) == &block, "should return the pointer we passed in");
	ASSERT( block.storage == &system, "should be the pointer we passed in");
	ASSERT( block.numStates == count, "should be %d", count );
	ASSERT( block.numInputs == system.numInputs, "should be %zu", system.numInputs );
	ASSERT( block.f, "should not be NULL" );

	FLOAT_TYPE dState[count] = {0};
	FLOAT_TYPE state[count];
	FLOAT_TYPE input[1] = {1};
	FLOAT_TYPE time = 10.0;

	for (size_t i = 0; i<count; i++)
		state[i] = i;

printf("hi1\n");
	block.f( &block, dState, time, state, input );
	ASSERT( dState[0] == input[0], "should just move input to dState" )
	for (size_t i = 1; i<count; i++)
	{
		ASSERT( dState[i] == state[i], "should just move input (state in this case) to dState %zu", i )
	}
	PASS;
}
