#include <assert.h>
#include <string.h>
#include "StrictlyProperBlock.h"

void euler
()
{

}

static void rk4_physics_step
(
	size_t const xi,
	double * const dX, // 5 * num_X buffer for rk4 algorithm
	double * const Xi, // 1 * num_X current state
	double const ti, // current time
	double const dt, // time step
	double const dt2, // time step / 2
	size_t const ui,
	double const * const U, // 3 * num_U input at time t, t + dt/2, t + dt
	PhysicsFunction const f,
	void * const storage
)
{
	size_t i;
	double * const A = &dX[0 * xi];
	double * const B = &dX[1 * xi];
	double * const C = &dX[2 * xi];
	double * const D = &dX[3 * xi];
	double * const Xt = &dX[4 * xi];
	double const * const Uti = &U[0 * ui];
	double const * const Ut2 = &U[1 * ui];
	double const * const Ut1 = &U[2 * ui];
	double const t2 = ti + dt2;
	double const t1 = ti + dt;

	f(xi, A, ti, xi, Xi, ui, Uti, storage);
	for (i = 0; i < xi; i++)
		Xt[i] = Xi[i] + A[i] * dt2;
	f(xi, B, t2, xi, Xt, ui, Ut2, storage);
	for (i = 0; i < xi; i++)
		Xt[i] = Xi[i] + B[i] * dt2;
	f(xi, C, t2, xi, Xt, ui, Ut2, storage);
	for (i = 0; i < xi; i++)
		Xt[i] = Xi[i] + C[i] * dt;
	f(xi, D, t1, xi, Xt, ui, Ut1, storage);
	for (i = 0; i < xi; i++)
		Xi[i] += dt * (A[i] + 2 * B[i] + 2 * C[i] + D[i]) / 6;

	//for i = 1:numpts - 1
	//	[A, output(i, :)] = f.physics(time(i), state(i, :), input(i, :));

	//B = f.physics(t2(i), state(i, :) + A*dt2, input2(i, :));
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