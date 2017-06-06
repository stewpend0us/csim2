#include <string.h>
#include "mexshared.h"


char * assertIsChar(char const name[STR1], mxArray const * const pr)
{
	char msg[STR2];
	if (!mxIsChar(pr))
	{
		strcpy_s(msg, STR2, name);
		strcat_s(msg, STR2, " must be a string.");
		MATLABERROR("notString", msg);
	}
	return mxArrayToString(pr);
}

double assertIsScalar(char const name[STR1], mxArray const * const pr)
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

double * assertIsVector(char const name[STR1], mxArray const * const pr)
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

double * assertIsMatrix(char const name[STR1], mxArray const * const pr)
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