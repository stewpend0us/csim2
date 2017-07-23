#include <math.h>
#include <assert.h>
#include "inverted_pendulum_example.h"

// First we'll need to declare the physics function and output function for our inverted_pendulum block
// Look in StrictlyProperBlock.h for the function signature we must use.
// Decalre them as static since no-one needs access to these functions outside this file. You'll be able to get at them via the StrictlyProperBlock interface.

static void physics(
	size_t const numStates,
	size_t const numInputs,
	double * const dState,
	double const time,
	double const * const state,
	double const * const input,
	struct inverted_pendulum_data * const storage //note the function signature uses void * here but we know what we're being passed so use that instead
);

static void output(
	size_t const numStates,
	size_t const numOutputs,
	double * const output,
	double const time,
	double const * const state,
	struct inverted_pendulum_data * const storage //same thing applies here
);

// Now we can easilly write the constructor for our block:
struct StrictlyProperBlock inverted_pendulum(struct inverted_pendulum_data * storage)
{
	struct StrictlyProperBlock block;
	block.numStates = 4; // cart position and velocity, and pendulum angle and angular rate
	block.numInputs = 1; // input the force acting on the cart
	block.numOutputs = 2; // output cart position and pendulum angle
	block.f = physics; // physics function pointer
	block.h = output; // output function pointer
	block.storage = storage; // pointer to our storage data
	return block;
}

// Now for the fun part! Implement the physics function:
static void physics(
	size_t const numStates,
	size_t const numInputs,
	double * const dState,
	double const time,
	double const * const state,
	double const * const input,
	struct inverted_pendulum_data * const storage //note the function signature uses void * here but we know what we're being passed so use that instead
)
{
	// Using the equations from wikipedia:
	//https://en.wikipedia.org/wiki/Inverted_pendulum#Equations_of_motion
	//		(M + m)*ddx - m*l*ddtheta*cos(theta) + m*l*dtheta^2*sin(theta) = F
	// and
	//		l*ddtheta - g*sin(theta) = ddx*cos(theta)
	//
	// these aren't in a directly implementable form so let's do some algebra
	//		ddx = (l*ddtheta - g*sin(theta))/cos(theta)
	// plug this in to the first equation and rearrange
	//		(M + m)*(l*ddtheta - g*sin(theta))/cos(theta) - m*l*ddtheta*cos(theta) + m*l*dtheta^2*sin(theta) = F
	//		(M + m)*l*ddtheta - (M + m)*g*sin(theta) - m*l*ddtheta*cos(theta)^2 + m*l*dtheta^2*sin(theta)*cos(theta) = F*cos(theta)
	//		((M + m)*l - m*l*cos(theta)^2)*ddtheta = F*cos(theta) + (M + m)*g*sin(theta) - m*l*dtheta^2*sin(theta)*cos(theta)
	//		ddtheta = (F*cos(theta) + (M + m)*g*sin(theta) - m*l*dtheta^2*sin(theta)*cos(theta))/((M + m)*l - m*l*cos(theta)^2)
	// now for the easy ones
	//		dtheta = thetadot;
	//		dx = xdot

	// do some quick checks
	assert(numStates == 4);
	assert(numInputs == 1);

	// decompose the state and dstate vectors
	double const x = state[0];
	double const xdot = state[1];
	double const theta = state[2];
	double const thetadot = state[3];
	double * const dx = &dState[0];
	double * const ddx = &dState[1];
	double * const dtheta = &dState[2];
	double * const ddtheta = &dState[3];

	// decompose the input vector
	double const F = input[0];

	// decompose storage
	double const l = storage->length;
	double const m = storage->mass;
	double const g = storage->gravity;
	double const M = storage->cart_mass;

	// make the sin and cos calls once
	double const ct = cos(theta);
	double const st = sin(theta);

	// solve for the state derivative
	*ddtheta = (F*ct + (M + m)*g*st - m*l*thetadot*thetadot * st*ct) / ((M + m)*l - m*l*ct*ct);
	*dtheta = thetadot;
	*ddx = (l*(*ddtheta) - g*st) / ct; // ideally we would only have 2 states and solve this exactly in the output function....
	*dx = xdot;
}

// and finally implement the output function
static void output(
	size_t const numStates,
	size_t const numOutputs,
	double * const output,
	double const time,
	double const * const state,
	struct inverted_pendulum_data * const storage //same thing applies here
)
{
	// quick checks
	assert(numStates == 4);
	assert(numOutputs == 2);

	// decompose the state vector
	double const x = state[0];
	//double const xdot = state[1];
	double const theta = state[2];
	//double const thetadot = state[3];

	// decompose the output vector
	double * const x_out = &output[0];
	double * const theta_out = &output[1];

	// calculate the output
	*x_out = x;
	*theta_out = theta;
}