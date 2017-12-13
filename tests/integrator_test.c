#include "minunit.h"
#include "integrator.h"
#include "multistep.h"

char * integrator_test()
{
	size_t const numBlocks = 2;

	struct StrictlyProperBlock const block = integrator(numBlocks,NULL);
	mu_assert(good_block(block),"block is bad");

	double const dt = .1;
	double const startTime = 5;
	double const duration = 30;
	double const stepTime = 10;
	mu_assert(stepTime >= startTime && stepTime < (startTime + duration - dt), "bad arguments");
	double const stepValue = 5;

	double * time = NULL;
	double * Xi = NULL;
	double * input = NULL;
	double * output = NULL;

	size_t const numSteps = numTimeSteps(dt, duration);
	time = malloc(numSteps * sizeof(double));
	check_mem(time);
	Xi = calloc(block.numStates, sizeof(double));
	check_mem(Xi);
	input = malloc(numSteps * block.numInputs * sizeof(double));
	check_mem(input);
	output = malloc(numSteps * block.numOutputs * sizeof(double));
	check_mem(output);

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

	euler(&block, output, dt, numSteps, time, Xi, input);
	size_t const last_step = numSteps - 1;
	mu_assert(last_step*dt + startTime == time[last_step], "Final time");
	mu_assert(stepValue == input[last_step*block.numInputs],"Final step value");
	for (size_t i = 0; i < block.numOutputs; i++)
	{
		mu_assert(((startTime + duration - stepTime)*stepValue + Xi[i]) == output[last_step*block.numOutputs + i],"Final output");
	}
	
	free(time);
	free(Xi);
	free(input);
	free(output);

	return NULL;
error:
	free(time);
	free(Xi);
	free(input);
	free(output);
	return "check failed";
}

RUN_TESTS(
	integrator_test,
);