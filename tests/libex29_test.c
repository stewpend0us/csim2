#include "minunit.h"

char *test_demo()
{
	mu_assert(1,"this is true");
	return NULL;
}

char * test_fail()
{
	mu_assert(0,"this should fail");
	return NULL;
}


RUN_TESTS(
	test_demo,
	test_fail,
);
