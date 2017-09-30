#include <stdbool.h>
#include "minunit.h"
#include "firstOrderLag.h"
#include "solvers.h"

#define tol 1e-10

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

	mu_assert(euler_results.increasing,		"euler: every output should be greater than the previous output");
	mu_assert(euler_results.right_size,		"euler: every output should be smaller than the input");
	mu_assert(euler_results.initial_value, 	"euler: the initial output should be equal to the initial condition");
	mu_assert(euler_results.final_value,	"euler: the final value should be within 'tol' of the input value");
	mu_assert(euler_results.right_trend,	"euler: the difference between outputs should be getting smaller");

	mu_assert(rk4_results.increasing,		"rk4: every output should be greater than the previous output");
	mu_assert(rk4_results.right_size,		"rk4: every output should be smaller than the input");
	mu_assert(rk4_results.initial_value,	"rk4: the initial output should be equal to the initial condition");
	mu_assert(rk4_results.final_value,		"rk4: the final value should be within 'tol' of the input value");
	mu_assert(rk4_results.right_trend,		"rk4: the difference between outputs should be getting smaller");

	mu_assert(rk4_smaller_than_euler, 		"rk4 is expected to have a smaller value than euler");

	return NULL;
}

RUN_TESTS(
	first_order_lag_step_test,
);