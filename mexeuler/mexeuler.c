#include <string.h>
#include <stdbool.h>
#include <Windows.h>
#include "mex.h"
#include "../csim2/solvers.h"

#define TOOLBOX "csim2"
#define FUNCTION "mexeuler"
#define DLLi 0
#define OBJi 1
#define DTi 2
#define Ti 3
#define ICi 4
#define Ui 5
#define DLLname "dllPath"
#define OBJname "objName"
#define DTname "dt"
#define Tname "time"
#define ICname "Xi"
#define Uname "input"
#define SIGNATURE "[ output, state, dstate ] = " FUNCTION "( " DLLname ", " OBJname", " DTname ", " Tname ", " ICname ", " Uname " )"

#define STR1 32
#define STR2 (STR1 + 64)
#define MATLABERROR(reason, msg) mexErrMsgIdAndTxt(TOOLBOX ":" FUNCTION ":" reason, msg)
#define MATLABASSERT(istrue, reason, msg) if (!(istrue)) { MATLABERROR(reason, msg); }

static HINSTANCE dllHandle = NULL;

static void onExitFreedllHandle(void)
{
	if (dllHandle)
	{
		FreeLibrary(dllHandle);
		dllHandle = NULL;
	}
}

static char * assertIsChar(char const name[STR1], mxArray const * const pr)
{
	char msg[STR2];
	if ( !mxIsChar(pr) )
	{
		strcpy_s(msg, STR2, name);
		strcat_s(msg, STR2, " must be a string.");
		MATLABERROR("notString", msg);
	}
	return mxArrayToString(pr);
}

static double assertIsScalar(char const name[STR1], mxArray const * const pr)
{
	char msg[STR2];
	if (
		!mxIsDouble(pr) ||
		mxIsComplex(pr) ||
		mxGetNumberOfElements(pr) != 1
		)
	{
		strcpy_s(msg, STR2, name);
		strcat_s(msg, STR2, " must be a scalar.");
		MATLABERROR("notScalar", msg);
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
		strcpy_s(msg, STR2, name);
		strcat_s(msg, STR2, " must be a matrix.");
		MATLABERROR("notVector", msg);
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
		strcpy_s(msg, STR2, name);
		strcat_s(msg, STR2, " must be a matrix.");
		MATLABERROR("notMatrix", msg);
	}
	return mxGetPr(pr);
}

void mexFunction(
	int nlhs,
	mxArray *plhs[],
	int nrhs,
	const mxArray *prhs[]
)
{
	MATLABASSERT(nrhs == 6, "nrhs", "6 inputs required: " SIGNATURE);
	MATLABASSERT(nlhs >= 1 && nlhs <= 3, "nlhs", "expected 1 to 3 outputs: " SIGNATURE);

	char * dll;
	char * obj;
	double dt; // 1 x 1 time step
	mwSize numSteps;
	double * time; // numSteps x 1 time vector
	mwSize numStates;
	double * Xi; // numStates x 1 initial conditions vector
	mwSize numInputs;
	double * U_t; // numSteps x numInputs inputs

	dll = assertIsChar(DLLname, prhs[DLLi]);
	obj = assertIsChar(OBJname, prhs[OBJi]);
	dt = assertIsScalar(DTname, prhs[DTi]);
	time = assertIsVector(Tname, prhs[Ti]);
	Xi = assertIsVector(ICname, prhs[ICi]);
	U_t = assertIsMatrix(Uname, prhs[Ui]);

	dllHandle = LoadLibrary(dll);
	if (!dllHandle)
	{
		char msg[STR2] = "failed to load dll \"";
		strcat_s(msg, STR2, dll);
		strcat_s(msg, STR2, "\"");
		MATLABERROR("failedToLoadDLL", msg);
	}
	mexAtExit(onExitFreedllHandle);

	StrictlyProperBlockDLLConstructor getBlock = (StrictlyProperBlockDLLConstructor)GetProcAddress(dllHandle, obj);
	if (!getBlock)
	{
		char msg[STR2] = "failed to load \"";
		strcat_s(msg, STR2, obj);
		strcat_s(msg, STR2, "\" from dll");
		MATLABERROR("failedToLoadStrictlyProperBlock", msg);
	}

	numSteps = mxGetNumberOfElements(prhs[Ti]);
	numStates = mxGetNumberOfElements(prhs[ICi]);
	numInputs = mxGetM(prhs[Ui]);

	MATLABASSERT(mxGetN(prhs[Ui]) == numSteps, "inconsistentInput", "size("Uname", 2) must equal numel("Tname").");

	struct StrictlyProperBlock const block = getBlock();

	if (block.numStates != numStates)
	{
		char msg[STR2];
		sprintf_s(msg, STR2, ICname " is expected to have %zu elements.", block.numStates);
		MATLABERROR("wrongSize_" ICname, msg);
	}
	
	if (block.numInputs != numInputs)
	{
		char msg[STR2];
		sprintf_s(msg, STR2, Uname " is expected to have %zu columns.", block.numInputs);
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
		memcpy(X, Xi, block.numStates * sizeof(double));
	}

	if (outputdState)
	{
		plhs[2] = mxCreateDoubleMatrix(block.numStates, numSteps, mxREAL);
		dX = mxGetPr(plhs[2]);
	}
	else
	{
		dX = mxMalloc(block.numStates * sizeof(double)); // matlab handles freeing this for us. how nice
	}

	//double * U = mxMalloc(numSteps * block.numInputs * sizeof(double));
	//for (size_t i = 0; i < numSteps; i++)
	//	for (size_t j = 0; j < block.numInputs; j++)
	//		U[i*block.numInputs + j] = U_t[i + j*numSteps];

	
	double * nextState = Xi;
	double const * currentState = Xi;
	double * currentdState = dX;
	double const * currentInput;

	size_t i = 0;
	size_t ic;
	block.h(block.numOutputs, &Y[i*block.numOutputs], time[i], block.numStates, currentState, block.storage);
	i++;
	for (; i < numSteps; i++)
	{
		ic = i - 1;
		if (outputState)
		{
			currentState = &X[ic*block.numStates];
			nextState = &X[i*block.numStates];
		}

		if (outputdState)
			currentdState = &dX[ic*block.numStates];

		currentInput = &U_t[ic*block.numInputs];
		
		euler_f_step(time[ic], dt, block.numStates, nextState, currentdState, currentState, block.numInputs, currentInput, block.f, block.storage);
		block.h(block.numOutputs, &Y[i*block.numOutputs], time[i], block.numStates, nextState, block.storage);
	}

	if (outputdState)
	{
		ic = i - 1;
		block.f(block.numStates, &dX[ic*block.numStates], time[ic], block.numStates, nextState, block.numInputs, &U_t[ic*block.numInputs], block.storage);
	}
	else
		mxFree(dX); // but the help says to do it anyway...
}

