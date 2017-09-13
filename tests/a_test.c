#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <assert.h>
#include "a_test.h"
#include "StrictlyProperBlock.h"
#include "integrator.h"
#include "firstOrderLag.h"
#include "solvers.h"

int main(void)
{
	size_t const numBlocks = 2;

	struct StrictlyProperBlock block = integrator(numBlocks);

	double const dt = .1;
	double const startTime = 5;
	double const duration = 30;
	double const stepTime = 10;
	assert(stepTime >= startTime && stepTime < (startTime + duration - dt));
	double const stepValue = 5;

	size_t const numSteps = numTimeSteps(dt, duration);
	double * const time = malloc(numSteps * sizeof(double));
	double * const Xi = calloc(block.numStates, sizeof(double));
	double * const input = malloc(numSteps * block.numInputs * sizeof(double));
	double * const output = malloc(numSteps * block.numOutputs * sizeof(double));

	initializeTime(numSteps, time, dt, startTime);

	for (size_t i = 0; i < numSteps; i++)
	{
		double value;
		if (time[i] < stepTime)
		{
			value = 0.0;
		}
		else
		{
			value = stepValue;
		}
		for (size_t j = 0; j < block.numInputs; j++)
		{
			input[i*block.numInputs + j] = value;
		}
	}

	euler(block, dt, numSteps, time, block.numStates, Xi, numBlocks, input, numBlocks, output);

	printf("%10s%10s", "time", "input");
	for (size_t i = 0; i < block.numOutputs; i++)
		printf("%9s%zu", "output", i);
	printf("\n");

	for (size_t i = 0; i < numSteps; i++)
	{
		printf("%10.2f%10.2f", time[i], input[i*block.numInputs]);
		for (size_t j = 0; j < block.numOutputs; j++)
		{
			printf("%10.4f", output[i*block.numOutputs + j]);
		}
		printf("\n");
	}

	printf("\nFinal row should be:\n");
	printf("%10.2f%10.2f", (numSteps - 1)*dt + startTime, stepValue);
	for (size_t i = 0; i < block.numOutputs; i++)
	{
		printf("%10.4f", (startTime + duration - stepTime)*stepValue + Xi[i]);
	}
	printf("\n\n\n");

	run_all_tests();
	printf("press enter\n");
	getchar();
}

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
		if (!(values[i] < value))
		{
			return false;
		}
	}
	return true;
}

static bool each_less_than_or_equal_to(size_t const numSteps, double const * const small, double const * const large)
{
	for (size_t i = 0; i < numSteps; i++)
	{
		if (!(small[i] <= large[i]))
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

struct first_order_lag_step_solver_results
{
	bool increasing;
	bool right_size;
	bool right_trend;
	bool initial_value;
	bool final_value;
};

static struct first_order_lag_step_solver_results check_first_order_lag_step
(
	size_t const numSteps,
	double const initialValue,
	double const finalValue,
	double const * const Y
)
{
	struct first_order_lag_step_solver_results result;

	result.increasing = is_increasing(numSteps, Y);
	result.right_size = all_less_than(numSteps, Y, finalValue);
	result.right_trend = difference_decreasing(numSteps, Y);
	result.initial_value = (Y[0] == initialValue);
	result.final_value = (finalValue - Y[numSteps - 1]) < tol;

	return result;
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
	rk4(block, dt, numSteps, time, block.numStates, Xi, block.numInputs, U_t, U_t, block.numOutputs, rk4Y);

	struct first_order_lag_step_solver_results euler_results = check_first_order_lag_step(numSteps, Xi[0], stepValue, eulerY);
	struct first_order_lag_step_solver_results rk4_results = check_first_order_lag_step(numSteps, Xi[0], stepValue, rk4Y);

	bool rk4_smaller_than_euler = each_less_than_or_equal_to(numSteps, rk4Y, eulerY);

	free(time);
	free(U_t);
	free(eulerY);
	free(rk4Y);

	assert_is_true(euler_results.increasing,	"euler: every output should be greater than the previous output");
	assert_is_true(euler_results.right_size,	"euler: every output should be smaller than the input");
	assert_is_true(euler_results.initial_value, "euler: the initial output should be equal to the initial condition");
	assert_is_true(euler_results.final_value,	"euler: the final value should be within 'tol' of the input value");
	assert_is_true(euler_results.right_trend,	"euler: the difference between outputs should be getting smaller");

	assert_is_true(rk4_results.increasing,		"rk4: every output should be greater than the previous output");
	assert_is_true(rk4_results.right_size,		"rk4: every output should be smaller than the input");
	assert_is_true(rk4_results.initial_value,	"rk4: the initial output should be equal to the initial condition");
	assert_is_true(rk4_results.final_value,		"rk4: the final value should be within 'tol' of the input value");
	assert_is_true(rk4_results.right_trend,		"rk4: the difference between outputs should be getting smaller");

	assert_is_true(rk4_smaller_than_euler, 		"rk4 is expected to have a smaller value than euler");

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
