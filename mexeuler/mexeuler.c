#include <string.h>
#include <stdbool.h>
#include <Windows.h>
#include "mex.h"
#include "../csim2/solvers.h"
#include "../csim2heap/dllInterface.h"
#define TOOLBOX "csim2"
#define FUNCTION "mexeuler"
#include "../mexshared/mexshared.h"
#define DLLi 0
#define OBJi 1
#define DTi 2
#define Ti 3
#define ICi 4
#define Ui 5
#define Oi 6
#define LASTi Oi
#define DLLname "dllPath"
#define OBJname "objName"
#define DTname "dt"
#define Tname "time"
#define ICname "Xi"
#define Uname "input"
#define Oname "options"
#define SIGNATURE "[ output, state, dstate ] = " FUNCTION "( " DLLname ", " OBJname", " DTname ", " Tname ", " ICname ", " Uname ", <" Oname "> )"

static HINSTANCE dllHandle = NULL;

static void onExitFreedllHandle(void)
{
	//mexPrintf("...\n");
	if (dllHandle)
	{
		FreeLibrary(dllHandle);
		dllHandle = NULL;
		//mexPrintf("FreeLibrary\n");
	}
}

void mexFunction(
	int nlhs,
	mxArray *plhs[],
	int nrhs,
	const mxArray *prhs[]
)
{
	MATLABASSERT(nrhs == LASTi || nrhs == (LASTi+1), "nrhs", "expected 6 to 7 inputs: " SIGNATURE);
	MATLABASSERT(nlhs >= 1 && nlhs <= 3, "nlhs", "expected 1 to 3 outputs: " SIGNATURE);

	char const * dll;
	char const * obj;
	double dt; // 1 x 1 time step
	mwSize numSteps;
	double const * time; // numSteps x 1 time vector
	mwSize numStates;
	double const * Xi; // numStates x 1 initial conditions vector
	mwSize numInputs;
	double const * U_t; // numSteps x numInputs inputs
	char const * options;
	char Empty = '\0';

	dll = assertIsChar(DLLname, prhs[DLLi]);
	obj = assertIsChar(OBJname, prhs[OBJi]);
	dt = assertIsScalar(DTname, prhs[DTi]);
	time = assertIsVector(Tname, prhs[Ti]);
	Xi = assertIsVector(ICname, prhs[ICi]);
	U_t = assertIsMatrix(Uname, prhs[Ui]);

	if (nrhs == (LASTi + 1))
		options = assertIsChar(Oname, prhs[Oi]);
	else
		options = &Empty;

	dllHandle = LoadLibrary(dll);
	if (!dllHandle)
	{
		char msg[STR2] = "failed to load dll \"";
		strcat_s(msg, STR2, dll);
		strcat_s(msg, STR2, "\"");
		MATLABERROR("failedToLoadDLL", msg);
	}
	mexAtExit(onExitFreedllHandle);

	struct dllStrictlyProperBlock * getBlock = (struct dllStrictlyProperBlock *)GetProcAddress(dllHandle, obj);
	if (!getBlock)
	{
		char msg[STR2] = "failed to load \"";
		strcat_s(msg, STR2, obj);
		strcat_s(msg, STR2, "\" from dll");
		onExitFreedllHandle();
		MATLABERROR("failedToLoadStrictlyProperBlock", msg);
	}

	struct StrictlyProperBlock * blockp = getBlock->constructor(options);
	if (!blockp)
	{
		char msg[STR2] = "failed to construct \"";
		strcat_s(msg, STR2, obj);
		strcat_s(msg, STR2, "\" from dll");
		onExitFreedllHandle();
		MATLABERROR("failedToConstructStrictlyProperBlock", msg);
	}
	struct StrictlyProperBlock block = *blockp;

	numSteps = mxGetNumberOfElements(prhs[Ti]);
	numStates = mxGetNumberOfElements(prhs[ICi]);
	numInputs = mxGetM(prhs[Ui]);

	MATLABASSERTf(onExitFreedllHandle, mxGetN(prhs[Ui]) == numSteps, "inconsistentInput", "size("Uname", 2) must equal numel("Tname").");

	if (block.numStates != numStates)
	{
		char msg[STR2];
		sprintf_s(msg, STR2, ICname " is expected to have %zu elements.", block.numStates);
		onExitFreedllHandle();
		MATLABERROR("wrongSize_" ICname, msg);
	}

	if (block.numInputs != numInputs)
	{
		char msg[STR2];
		sprintf_s(msg, STR2, Uname " is expected to have %zu columns.", block.numInputs);
		onExitFreedllHandle();
		MATLABERROR("wrongSize_" Uname, msg);
	}

	// done with checking the inputs.
	// set up the outputs:

	bool const outputState = (nlhs >= 2);
	bool const outputdState = (nlhs >= 3);

	double * Y; // numSteps x numOutputs
	double * X = NULL; // numSteps x numStates OR 1 x numStates
	double * dX; // numSteps x numStates OR 1 x numStates

	plhs[0] = mxCreateDoubleMatrix(block.numOutputs, numSteps, mxREAL);
	Y = mxGetPr(plhs[0]);

	if (outputState)
	{
		plhs[1] = mxCreateDoubleMatrix(block.numStates, numSteps, mxREAL);
		X = mxGetPr(plhs[1]);
	}
	else
	{
		X = mxMalloc(block.numStates * sizeof(double));
	}
	memcpy(X, Xi, block.numStates * sizeof(double));

	if (outputdState)
	{
		plhs[2] = mxCreateDoubleMatrix(block.numStates, numSteps, mxREAL);
		dX = mxGetPr(plhs[2]);
	}
	else
	{
		dX = mxMalloc(block.numStates * sizeof(double)); // matlab handles freeing this for us. how nice
	}

	// done with setting up the outputs
	// solve the problem

	double * nextState = X;
	double const * currentState = X;
	double * currentdState = dX;
	double const * currentInput;
	double * currentOutput;
	size_t i;
	for (i = 0; i < (numSteps - 1); i++)
	{
		if (outputState)
		{
			currentState = &X[i*block.numStates];
			nextState = &X[(i + 1)*block.numStates];
		}

		if (outputdState)
			currentdState = &dX[i*block.numStates];

		currentInput = &U_t[i*block.numInputs];
		currentOutput = &Y[i*block.numOutputs];

		block.h(block.numStates, block.numOutputs, currentOutput, time[i], currentState, block.storage);
		euler_f_step(block.numStates, block.numInputs, dt, time[i], nextState, currentdState, currentState, currentInput, block.f, block.storage);
	}

	//i = numSteps - 1;
	currentState = nextState;
	currentOutput = &Y[i*block.numOutputs];
	block.h(block.numStates, block.numOutputs, currentOutput, time[i], currentState, block.storage);

	if (outputdState)
	{
		currentdState = &dX[i*block.numStates];
		currentInput = &U_t[i*block.numInputs];
		block.f(block.numStates, block.numInputs, currentdState, time[i], currentState, currentInput, block.storage);
	}
	else
		mxFree(dX);

	if (!outputState)
		mxFree(X);

	getBlock->destructor(blockp);
	onExitFreedllHandle();
}

