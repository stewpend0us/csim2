#include "minunit.h"
#include "integrator.h"
#include "solvers.h"

char * integrator_test()
{
	size_t const numBlocks = 2;

	struct StrictlyProperBlock const block = integrator(numBlocks);
	struct StrictlyProperBlockInfo const bi = block.info;

	double const dt = .1;
	double const startTime = 5;
	double const duration = 30;
	double const stepTime = 10;
	mu_assert(stepTime >= startTime && stepTime < (startTime + duration - dt), "bad arguments");
	double const stepValue = 5;

	size_t const numSteps = numTimeSteps(dt, duration);
	double * const time = malloc(numSteps * sizeof(double));
	double * const Xi = calloc(bi.numStates, sizeof(double));
	double * const input = malloc(numSteps * bi.numInputs * sizeof(double));
	double * const output = malloc(numSteps * bi.numOutputs * sizeof(double));

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
		for (size_t j = 0; j < bi.numInputs; j++)
		{
			input[i*bi.numInputs + j] = value;
		}
	}

	euler(&block, output, dt, numSteps, time, Xi, input);
	size_t const last_step = numSteps - 1;
	mu_assert(last_step*dt + startTime == time[last_step], "Final time");
	mu_assert(stepValue == input[last_step*bi.numInputs],"Final step value");
	for (size_t i = 0; i < bi.numOutputs; i++)
	{
		mu_assert(((startTime + duration - stepTime)*stepValue + Xi[i]) == output[last_step*bi.numOutputs + i],"Final output");
	}
	
	return NULL;
}

RUN_TESTS(
	integrator_test,
);