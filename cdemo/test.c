#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include "test.h"
#include "../csim2/solvers.h"
#include "../csim2/firstOrderLag.h"

#define tol 1e-10

static char * test_pass()
{
	assert_is_true(true, "should pass");
	return NULL;
}

static char * test_fail()
{
	assert_is_true(false, "should fail");
	return NULL;
}

static bool is_increasing(size_t const numSteps, double const * const values)
{
	for (size_t i = 1; i < numSteps; i++)
	{
		if ((values[i] - values[i - 1]) < 0.0)
		{
			return false;
		}
	}
	return true;
}

static bool all_less_than(size_t const numSteps, double const * const values, double value)
{
	for (size_t i = 0; i < numSteps; i++)
	{
		if (values[i] > value)
		{
			return false;
		}
	}
	return true;
}

static bool difference_decreasing(size_t numSteps, double const * const values)
{
	if (numSteps > 2)
	{
		double prevDiff = values[1] - values[0];
		for (size_t i = 2; i < numSteps; i++)
		{
			double diff = values[i] - values[i - 1];
			if (diff > prevDiff)
				return false;
			prevDiff = diff;
		}
	}
	return true;
}

static char * first_order_lag_step_test()
{
	double tau[1] = { 3 };
	struct StrictlyProperBlock block = firstOrderLag(1, tau);

	double const dt = .01;
	double const startTime = 0;
	double const duration = 300;
	double const stepValue = 5;

	size_t const numSteps = numTimeSteps(dt, duration);
	double * const time = malloc(numSteps * sizeof(double));
	double * const U_t = malloc(numSteps * 1 * sizeof(double));
	double * const eulerY = malloc(numSteps * 1 * sizeof(double));
	double * const rk4Y = malloc(numSteps * 1 * sizeof(double));
	initializeTime(numSteps, time, dt, startTime);
	for (size_t i = 0; i < numSteps; i++)
	{
		U_t[i] = stepValue;
	}

	double Xi[1] = { 0 };

	euler(block, dt, numSteps, time, block.numStates, Xi, block.numInputs, U_t, block.numOutputs, eulerY);

	bool increasing = is_increasing(numSteps, eulerY);
	bool right_size = all_less_than(numSteps, eulerY, stepValue);
	bool right_trend = difference_decreasing(numSteps, eulerY);
	bool initial_value = (eulerY[0] == Xi[0]);
	bool final_value = (stepValue - eulerY[numSteps - 1]) < tol;

	free(time);
	free(U_t);
	free(eulerY);
	free(rk4Y);

	assert_is_true(increasing, "every output should be greater than the previous output");
	assert_is_true(right_size, "every output should be smaller than the input");
	assert_is_true(initial_value, "the initial output should be equal to the initial condition");
	assert_is_true(final_value, "the final value should be within 'tol' of the input value");
	assert_is_true(right_trend, "the difference between outputs should be getting smaller");
	return NULL;
}

void run_all_tests()
{
	test_function tests[] = 
	{
		test_pass,
		test_fail,
		first_order_lag_step_test,
	};
	run_tests(sizeof(tests) / sizeof(test_function), tests);
}

void run_tests(size_t count, test_function * tests)
{
	printf("TESTS:\n");
	for (size_t i = 0; i < count; i++)
	{
		clock_t start = clock();
		char * result = tests[i]();
		clock_t stop = clock();
		double time_spent = (double)(stop - start) / CLOCKS_PER_SEC * 1000;
		printf("%4zu of %zu took %5.2f ms: ", i+1, count, time_spent);
		if (result)
			printf("FAILED %s\n", result);
		else
			printf("passed\n");
	}
}
