#include "integrator.h"
#include "blockSystem.h"
#include "test.h"
#include <string.h>

void update
(
	struct blockSystem const * blockSystem,
	FLOAT_TYPE time,
	FLOAT_TYPE * childState[], // array of child block state arrays
	FLOAT_TYPE const systemInput[]
)
{
}

int main(void)
{
	struct block block;
	struct blockSystem system;
	#define count 3
	struct block child[count] = {0};
	FLOAT_TYPE * childInput[count] = {0};

	FLOAT_TYPE dState[count] = {0};
	FLOAT_TYPE state[count] = {0};
	FLOAT_TYPE input[count] = {0};
	FLOAT_TYPE tau[count] = {0};
	FLOAT_TYPE time = 10.0;

	ASSERT( !blockSystem( NULL, &system ), "should return NULL" );
	ASSERT( !blockSystem( &block, NULL ), "should return NULL" );
	system.numChildren = 0;
	system.child = child;
	system.childInput = childInput;
	ASSERT( !blockSystem( &block, &system ), "should return NULL" );
	system.numChildren = count;
	system.child = NULL;
	system.childInput = childInput;
	ASSERT( !blockSystem( &block, &system ), "should return NULL" );
	system.numChildren = count;
	system.child = child;
	system.childInput = NULL;
	ASSERT( !blockSystem( &block, &system ), "should return NULL" );
	system.numChildren = count;
	system.child = child;
	system.childInput = childInput;
	ASSERT( !blockSystem( &block, &system ), "should return NULL" );
	for (size_t i = 0; i<count; i++)
	{
		ASSERT( integrator( &child[i], 1 ) == &child[i], "failed to create integrator %zu", i );
	}
	ASSERT( !blockSystem( &block, &system ), "should return NULL" );
	for (size_t i = 0; i<count; i++)
	{
		childInput[i] = malloc( sizeof( FLOAT_TYPE ) );
		ASSERT( childInput[i], "malloc failed %zu", i );
	}
	ASSERT( blockSystem( &block, &system ) == &block, "should return the pointer we passed in");
	ASSERT( block.storage == &system, "should be the pointer we passed in");
	PASS;
}
