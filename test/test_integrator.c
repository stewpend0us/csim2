#include "integrator.h"
#include <stdio.h>

#define fail(fmt, ...) warnx("%d: " fmt, __LINE__, #__VA_ARGS__)
int main(void)
{
	struct block block;
	size_t count = 3;
	if ( integrator( NULL, 1 ) )
		printf(__FILE__":%d: " "should return NULL", __LINE__);
	if ( integrator( &block, 0 ) )
		fail("should return NULL");
	if ( integrator( &block, count ) != &block )
		fail("should return the pointer we passed in");
	if ( block.numStates != count )
		fail("should be %zu", count);
	if ( block.numInputs != count )
		fail("should be %zu", count);
	if ( block.storage )
		fail("should be NULL");
	if ( !block.f )
		fail("should not be NULL");

	warnx("passed");
}
