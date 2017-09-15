#include "minunit.h"

char *test_demo()
{
	printf("demo test\n");
	mu_assert(1,"this is true");
	return NULL;
}

char * test_fail()
{
	mu_assert(0,"this should fail");
	return NULL;
}

char *all_tests()
{
	mu_start();

	mu_run_test(test_demo);
	mu_run_test(test_fail);
	return NULL;
}

RUN_TESTS(all_tests);
