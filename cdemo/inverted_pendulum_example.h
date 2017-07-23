#pragma once

#include "../csim2/StrictlyProperBlock.h"

//First declare a struct that contains all the data our physics function and output function might need
struct inverted_pendulum_data
{
	double length;
	double mass;
	double cart_mass;
	double gravity;
};

//Now declare the constructor for our new block type
struct StrictlyProperBlock inverted_pendulum(struct inverted_pendulum_data * storage);
