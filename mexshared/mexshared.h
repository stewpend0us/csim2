#pragma once
#include "mex.h"

#ifndef TOOLBOX
#define TOOLBOX "toolbox"
#endif

#ifndef FUNCTION
#define FUNCTION "function"
#endif

#define STR1 32
#define STR2 (STR1 + 96)
#define MATLABERROR(reason, msg) mexErrMsgIdAndTxt(TOOLBOX ":" FUNCTION ":" reason, msg)
#define MATLABASSERT(istrue, reason, msg) if (!(istrue)) { MATLABERROR(reason, msg); }
#define MATLABASSERTf(f, istrue, reason, msg) if (!(istrue)) { f(); MATLABERROR(reason, msg); }

char * assertIsChar(char const name[STR1], mxArray const * const pr);
double assertIsScalar(char const name[STR1], mxArray const * const pr);
double * assertIsVector(char const name[STR1], mxArray const * const pr);
double * assertIsMatrix(char const name[STR1], mxArray const * const pr);