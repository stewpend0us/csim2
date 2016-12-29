#pragma once
#include "user_types.h"

typedef struct
{
	double t;
	user_state ustate;
	user_input uinput;
	user_output uoutput;
	double * sstate;
	double * sinput;
	double * soutput;
	void * solver_storage;
} solver_storage;

typedef void(*solver_initialize)(solver_storage*, user_system*, user_state, user_input, user_output);
typedef void(*solver_function)(user_system*, solver_storage*);