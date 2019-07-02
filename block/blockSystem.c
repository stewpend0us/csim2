#include "blockSystem.h"

static void physics
(
	FLOAT_TYPE time,
	size_t num_states,
	FLOAT_TYPE dstate[],
	FLOAT_TYPE const state[],
	size_t num_inputs,
	FLOAT_TYPE input[],
	void * storage
)
{
	(void)num_states;
	(void)num_inputs;
	struct blockSystem * system = storage;
	size_t i, xi, ui, num_system_inputs = system->num_inputs, num_children = system->num_children;
	struct block * c = system->child;
	FLOAT_TYPE ** c_input = system->child_input;
	FLOAT_TYPE const ** c_state = (FLOAT_TYPE const **)system->child_state;
	for ( i = 0, xi = 0, ui = num_system_inputs; i < num_children; xi += c[i].num_states, ui += c[i].num_inputs, i++ )
	{
		c_state[i] = &state[xi];
		c_input[i] = &input[ui];
	}
	system->updateChildInputs( time, num_children, c, c_input, c_state, num_system_inputs, input, system->storage );
	for ( i = 0, xi = 0; i < num_children; xi += c[i++].num_states )
		c[i].f(time, c[i].num_states, &dstate[xi], c_state[i], c[i].num_inputs, c_input[i], c[i].storage);
}

struct block * blockSystem( struct block * block, struct blockSystem * system )
{
	if ( !block || !system )
		return NULL;

	struct block * child = system->child;
	size_t num_children = system->num_children;
	if ( !child || !num_children || !system->child_state || !system->child_input || !system->updateChildInputs )
		return NULL;

	size_t total_states = 0;
	size_t total_inputs = system->num_inputs;
	for (size_t i = 0; i < num_children; i++)
	{
		if ( !child[i].num_states || !child[i].f)
			return NULL;
		total_states += child[i].num_states;
		total_inputs += child[i].num_inputs;
	}
	block->num_states = total_states;
	block->num_inputs = total_inputs;
	block->storage = system;
	block->f = physics;
	return block;
}
