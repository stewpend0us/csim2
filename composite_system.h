#pragma once
#include "user_types.h"

// a composite_system returns a user_system which 

typedef struct
{
	size_t num_systems;
	user_system * systems;
} composite_system_constants;

user_system composite_system(composite_system_constants * system_array);