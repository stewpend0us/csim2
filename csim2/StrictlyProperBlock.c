#include <assert.h>
#include <string.h>
#include <math.h>
#include "StrictlyProperBlock.h"

void euler_f_step
(
	double const ti, // current time
	double const dt, // time step
	size_t const xi,
	double * const Xt1, // (1 * xi) next State
	double * const A, // (1 * xi) current dstate
	double const * const Xti, // (1 * xi) current state/initial conditions
	size_t const ui,
	double const * const Uti, // (1 * ui) input at time ti
	PhysicsFunction const f,
	void * const storage
)
{
	size_t i;

	f(xi, A, ti, xi, Xti, ui, Uti, storage);
	for (i = 0; i < xi; i++)
		Xt1[i] = Xti[i] + A[i] * dt;
}

void rk4_f_step
(
	double const ti, // current time
	double const dt, // time step
	size_t const xi,
	double * const Xt1, // (1 * xi) next State
	double * const A, // (1 * xi) current dstate
	double * const B, // (1 * xi) solver storage/temp
	double * const C, // (1 * xi) solver storage/temp
	double * const D, // (1 * xi) solver storage/temp
	double * const Xtmp, // (1 * xi) solver storage/temp
	double const * const Xti, // (1 * xi) current state/initial conditions
	size_t const ui,
	double const * const Uti, // (1 * ui) input at time ti
	double const * const Ut2, // (1 * ui) input at time ti + dt/2
	double const * const Ut1, // (1 * ui) input at time ti + dt
	PhysicsFunction const f,
	void * const storage
)
{
	assert(Xtmp != Xt1);
	assert(Xtmp != A);
	assert(Xtmp != B);
	assert(Xtmp != C);
	assert(Xtmp != D);
	assert(Xtmp != Xti);

	size_t i;
	double const dt2 = dt / 2;
	double const t2 = ti + dt2;
	double const t1 = ti + dt;

	f(xi, A, ti, xi, Xti, ui, Uti, storage);

	for (i = 0; i < xi; i++)
		Xtmp[i] = Xti[i] + A[i] * dt2;
	f(xi, B, t2, xi, Xtmp, ui, Ut2, storage);

	for (i = 0; i < xi; i++)
		Xtmp[i] = Xti[i] + B[i] * dt2;
	f(xi, C, t2, xi, Xtmp, ui, Ut2, storage);

	for (i = 0; i < xi; i++)
		Xtmp[i] = Xti[i] + C[i] * dt;
	f(xi, D, t1, xi, Xtmp, ui, Ut1, storage);

	for (i = 0; i < xi; i++)
		Xt1[i] = Xti[i] + dt * (A[i] + 2 * B[i] + 2 * C[i] + D[i]) / 6;
}

size_t numSteps
(
	double const ti,
	double const dt,
	double const tf
)
{
	return (size_t)floor((tf - ti) / dt + 1);
}