#pragma once
#include <stddef.h>

// user defined types
typedef void * user_dstate;
typedef void * user_state;
typedef void * user_input;
typedef void * user_output;
typedef void * user_system_constants;

// user defined system functions
typedef void(*user_dstate_function)(double t, user_state, user_input, user_output, user_system_constants, user_dstate);
typedef void(*user_input_function)(double t, user_state, user_input, user_system_constants);
typedef void(*user_output_function)(double t, user_state, user_input, user_output, user_system_constants);
typedef void(*user_once_per_step_function)(double t, user_state, user_input, user_output, user_system_constants);

// user defined functions to convert between user defined types and double array
typedef void(*user_state_to_state)(double * state, user_state, user_system_constants);
typedef void(*user_input_to_input)(double * input, user_input, user_system_constants);
typedef void(*user_output_to_output)(double * output, user_output, user_system_constants);
typedef void(*state_to_user_state)(user_state, double * state, user_system_constants);
typedef void(*input_to_user_input)(user_input, double * input, user_system_constants);
typedef void(*output_to_user_output)(user_output, double * output, user_system_constants);

typedef struct
{
	size_t num_states;
	size_t num_inputs;
	size_t num_outputs;
	user_system_constants constants;
	user_dstate_function dstate;
	user_input_function input;
	user_output_function output;
	user_once_per_step_function once_per_step;
	user_state_to_state to_state;
	user_input_to_input to_input;
	user_output_to_output to_output;
	state_to_user_state to_user_state;
	input_to_user_input to_user_input;
	output_to_user_output to_user_output;
} user_system;