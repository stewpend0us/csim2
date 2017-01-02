#pragma once
#include "composite_system.h"

// prototypes: 

typedef user_dstate * composite_dstate;
typedef user_state * composite_state;
typedef user_input * composite_input;
typedef user_output * composite_output;

void composite_dstate_function(double t, composite_state, composite_input, composite_output, composite_system_constants, composite_dstate);
void composite_input_function(double t, composite_state, composite_input, composite_system_constants);
void composite_output_function(double t, composite_state, composite_input, composite_output, composite_system_constants);
void composite_once_per_step_function(double t, composite_state, composite_input, composite_output, composite_system_constants);

void composite_state_to_state(double * state, composite_state, composite_system_constants);
void composite_input_to_input(double * input, composite_input, composite_system_constants);
void composite_output_to_output(double * output, composite_output, composite_system_constants);
void state_to_composite_state(composite_state, double * state, composite_system_constants);
void input_to_composite_input(composite_input, double * input, composite_system_constants);
void output_to_composite_output(composite_output, double * output, composite_system_constants);

// definitions:

void composite_dstate_function(double t, composite_state cstates, composite_input cinputs, composite_output coutputs, composite_system_constants cconstants, composite_dstate cdstates)
{
	for (size_t i = 0; i < cconstants.num_systems; i++)
	{
		cconstants.systems[i].dstate(t, cstates[i], cinputs[i], coutputs[i], cconstants.systems[i].constants, cdstates[i]);
	}
}

void composite_input_function(double t, composite_state cstates, composite_input cinputs, composite_system_constants cconstants)
{
	for (size_t i = 0; i < cconstants.num_systems; i++)
	{
		cconstants.systems[i].input(t, cstates[i], cinputs[i], cconstants.systems[i].constants);
	}
}

void composite_output_function(double t, composite_state cstates, composite_input cinputs, composite_output coutputs, composite_system_constants cconstants)
{
	for (size_t i = 0; i < cconstants.num_systems; i++)
	{
		cconstants.systems[i].output(t, cstates[i], cinputs[i], coutputs[i], cconstants.systems[i].constants);
	}
}

void composite_once_per_step_function(double t, composite_state cstates, composite_input cinputs, composite_output coutputs, composite_system_constants cconstants)
{
	for (size_t i = 0; i < cconstants.num_systems; i++)
	{
		cconstants.systems[i].once_per_step(t, cstates[i], cinputs[i], coutputs[i], cconstants.systems[i].constants);
	}
}

void composite_state_to_state(double * state, composite_state cstates, composite_system_constants cconstants)
{
	size_t j = 0;
	for (size_t i = 0; i < cconstants.num_systems; i++)
	{
		cconstants.systems[i].to_state(&state[j], cstates[i], cconstants.systems[i].constants);
		j += cconstants.systems[i].num_states;
	}
}

void composite_input_to_input(double * input, composite_input cinputs, composite_system_constants cconstants)
{
	size_t j = 0;
	for (size_t i = 0; i < cconstants.num_systems; i++)
	{
		cconstants.systems[i].to_input(&input[j], cinputs[i], cconstants.systems[i].constants);
		j += cconstants.systems[i].num_inputs;
	}
}

void composite_output_to_output(double * output, composite_output coutputs, composite_system_constants cconstants)
{
	size_t j = 0;
	for (size_t i = 0; i < cconstants.num_systems; i++)
	{
		cconstants.systems[i].to_output(&output[j], coutputs[i], cconstants.systems[i].constants);
		j += cconstants.systems[i].num_outputs;
	}
}

void state_to_composite_state(composite_state cstates, double * state, composite_system_constants cconstants)
{
	size_t j = 0;
	for (size_t i = 0; i < cconstants.num_systems; i++)
	{
		cconstants.systems[i].to_user_state(cstates[i], &state[j], cconstants.systems[i].constants);
		j += cconstants.systems[i].num_states;
	}
}

void input_to_composite_input(composite_input cinputs, double * input, composite_system_constants cconstants)
{
	size_t j = 0;
	for (size_t i = 0; i < cconstants.num_systems; i++)
	{
		cconstants.systems[i].to_user_input(cinputs[i], &input[j], cconstants.systems[i].constants);
		j += cconstants.systems[i].num_inputs;
	}
}

void output_to_composite_output(composite_output coutputs, double * output, composite_system_constants cconstants)
{
	size_t j = 0;
	for (size_t i = 0; i < cconstants.num_systems; i++)
	{
		cconstants.systems[i].to_user_output(coutputs[i], &output[j], cconstants.systems[i].constants);
		j += cconstants.systems[i].num_outputs;
	}
}

user_system composite_system(composite_system_constants * system_array)
{
	user_system r;
	r.num_states = 0;
	r.num_inputs = 0;
	r.num_outputs = 0;
	r.constants = system_array;

	for (size_t i = 0; i < system_array->num_systems; i++)
	{
		r.num_states += system_array->systems[i].num_states;
		r.num_inputs += system_array->systems[i].num_inputs;
		r.num_outputs += system_array->systems[i].num_outputs;
	}
	
	r.dstate = &composite_dstate_function;
	r.input = &composite_input_function;
	r.output = &composite_output_function;
	r.once_per_step = &composite_once_per_step_function;

	r.to_state = &composite_state_to_state;
	r.to_input = &composite_input_to_input;
	r.to_output = &composite_output_to_output;

	r.to_user_state = &state_to_composite_state;
	r.to_user_input = &input_to_composite_input;
	r.to_user_output = &output_to_composite_output;

	return r;
}