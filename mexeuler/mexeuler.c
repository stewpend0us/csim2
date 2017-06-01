#include <string.h>
#include <stdbool.h>
#include "mex.h"
#include "../csim2/solvers.h"
#include "../csim2/integrator.h"

#define STR1 32
#define STR2 (STR1 + 64)
#define TOOLBOX "csim2"
#define FUNCTION "mexeuler"
#define SIGNATURE "[output, state, dstate] = " FUNCTION "(dt, time, Xi, input)"
#define ERROR(reason, msg) mexErrMsgIdAndTxt(TOOLBOX ":" FUNCTION ":" reason, msg)
#define ASSERT(istrue, reason, msg) if (!(istrue)) { ERROR(reason, msg); }

static double assertIsScalar(char const name[STR1], mxArray const * const pr)
{
	char msg[STR2];
	if (
		!mxIsDouble(pr) ||
		mxIsComplex(pr) ||
		mxGetNumberOfElements(pr) != 1
		)
	{
		strcpy(msg, name);
		strcat(msg, " must be a scalar.");
		ERROR("notScalar", msg);
	}
	return mxGetScalar(pr);
}

static double * assertIsVector(char const name[STR1], mxArray const * const pr)
{
	char msg[STR2];
	if (
		!mxIsDouble(pr) ||
		mxIsComplex(pr) ||
		mxGetNumberOfDimensions(pr) != 2 ||
		!(mxGetM(pr) == 1 || mxGetN(pr) == 1)
		)
	{
		strcpy(msg, name);
		strcat(msg, " must be a matrix.");
		ERROR("notVector", msg);
	}
	return mxGetPr(pr);
}

static double * assertIsMatrix(char const name[STR1], mxArray const * const pr)
{
	char msg[STR2];
	if (
		!mxIsDouble(pr) ||
		mxIsComplex(pr) ||
		mxGetNumberOfDimensions(pr) != 2
		)
	{
		strcpy(msg, name);
		strcat(msg, " must be a matrix.");
		ERROR("notMatrix", msg);
	}
	return mxGetPr(pr);
}

static struct StrictlyProperBlock getBlock()
{
	return integrator(1);
}

void mexFunction(
	int nlhs,
	mxArray *plhs[],
	int nrhs,
	const mxArray *prhs[]
)
{
	ASSERT(nrhs == 4, "nrhs", "four inputs required: " SIGNATURE);
	ASSERT(nlhs >= 1 && nlhs <= 3, "nlhs", "expected one to three outputs: " SIGNATURE);

	double dt; // 1 x 1 time step
	mwSize numSteps;
	double * time; // numSteps x 1 time vector
	mwSize numStates;
	double * Xi; // numStates x 1 initial conditions vector
	mwSize numInputs;
	double * U; // numSteps x numInputs inputs

	dt = assertIsScalar("dt", prhs[0]);
	time = assertIsVector("time", prhs[1]);
	Xi = assertIsVector("Xi", prhs[2]);
	U = assertIsMatrix("input", prhs[3]);

	numSteps = mxGetNumberOfElements(prhs[1]);
	numStates = mxGetNumberOfElements(prhs[2]);
	numInputs = mxGetN(prhs[3]);

	ASSERT(mxGetM(prhs[3]) == numSteps, "inconsistentInput", "size(input,1) must equal numel(time).");

	struct StrictlyProperBlock block = getBlock();

	if (block.numStates != numStates)
	{
		char msg[STR2];
		sprintf(msg, "Xi is expected to have %zu elements.", block.numStates);
		ERROR("wrongNumStates", msg);
	}
	
	if (block.numInputs != numInputs)
	{
		char msg[STR2];
		sprintf(msg, "input is expected to have %zu columns.", block.numInputs);
		ERROR("wrongNumInputs", msg);
	}
	
	// done with checking the inputs. set up the outputs
	bool const outputState = (nlhs >= 2);
	bool const outputdState = (nlhs >= 3);

	double * Y; // numSteps x numOutputs
	double * X; // numSteps x numStates OR 1 x numStates
	double * dX; // numSteps x numStates OR 1 x numStates

	plhs[0] = mxCreateDoubleMatrix(numSteps, block.numOutputs, mxREAL);
	Y = mxGetPr(plhs[0]);

	if (outputState)
	{
		plhs[1] = mxCreateDoubleMatrix(numSteps, block.numStates, mxREAL);
		X = mxGetPr(plhs[1]);
		memcpy(X, Xi, block.numStates * sizeof(double));
	}

	if (outputdState)
	{
		plhs[2] = mxCreateDoubleMatrix(numSteps, block.numStates, mxREAL);
		dX = mxGetPr(plhs[2]);
	}
	else
	{
		dX = mxMalloc(block.numStates * sizeof(double)); // matlab handles freeing this for us. how nice
	}

	double * nextState = Xi;
	double const * currentState = Xi;
	double * currentdState = dX;
	double const * currentInput;

	size_t i = 0;
	block.h(block.numOutputs, &Y[i*block.numOutputs], time[i], block.numStates, currentState, block.storage);
	i++;
	for (; i < numSteps; i++)
	{
		if (outputState)
		{
			currentState = &X[(i - 1)*block.numStates];
			nextState = &X[i*block.numStates];
		}

		if (outputdState)
			currentdState = &dX[(i - 1)*block.numStates];

		currentInput = &U[(i - 1)*block.numInputs];
		euler_f_step(time[i], dt, block.numStates, nextState, currentdState, currentState, block.numInputs, currentInput, block.f, block.storage);
		block.h(block.numOutputs, &Y[i*block.numOutputs], time[i], block.numStates, currentState, block.storage);
	}
}

