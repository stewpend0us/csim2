#include <assert.h>
#include <string.h>
#include "StrictlyProperBlock.h"

void euler
()
{

}

void rk4
(
	double * const Y,
	double * const Xi,
	struct StrictlyProperBlock const * const block,
	size_t const num_t,
	double const * const t,
	double const * const U
)
{

	size_t yi = block->numOutputs;
	size_t xi = block->numStates;
	void * storage = block->storage;

	for (size_t i = 0; i < num_t; i++)
	{
		block->h(yi, &Y[i*yi], t[i], xi, Xi, storage);

	}
	//for i = 1:numpts - 1
	//	[A, output(i, :)] = f.physics(time(i), state(i, :), input(i, :));

	//if endEarly(i, time, state, output)
	//	beenEnded = true;
	//break;
	//end
	//	B = f.physics(t2(i), state(i, :) + A*dt2, input2(i, :));
	//C = f.physics(t2(i), state(i, :) + B*dt2, input2(i, :));
	//D = f.physics(time(i + 1), state(i, :) + C*dt, input(i + 1, :));

	//state(i + 1, :) = state(i, :) + dt*(A + 2 * B + 2 * C + D) / 6;
	//dstate(i, :) = A;

	//if any(isnan(state(i, :))) && ~beenWarned
	//	warning('state contains NaN at time %g', time(i));
	//beenWarned = true;
	//end
	//	end
}