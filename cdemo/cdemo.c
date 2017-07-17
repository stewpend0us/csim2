#include <stdio.h>
#include <assert.h>
#include "../csim2/StrictlyProperBlock.h"
#include "../csim2/integrator.h"
#include "../csim2/solvers.h"

int main(int argc, char * argv[])
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
	printf("%10.2f%10.2f", (numSteps-1)*dt + startTime, stepValue);
	for (size_t i = 0; i < block.numOutputs; i++)
	{
		printf("%10.4f", (startTime + duration - stepTime)*stepValue + Xi[i]);
	}
	printf("\n\n");

	printf("press enter\n");
	getchar();
}