#pragma once

#include "../csim2/StrictlyProperBlock.h"

//First declare a struct that contains all the data our physics function and output function might need
struct inverted_pendulum_data
{
	double length; // length of the pendulum
	double mass; // mass of the pendulum
	double cart_mass; // mass of the cart
	double gravity; // acceleration due to gravity
};

//Now declare the constructor for our new block type
struct StrictlyProperBlock inverted_pendulum(struct inverted_pendulum_data * storage);

