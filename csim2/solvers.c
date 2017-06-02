#include <math.h>
#include "solvers.h"

void euler_f_step
(
	size_t const xi,
	size_t const ui,
	double const dt, // time step
	double const ti, // current time
	double * const Xt1, // (1 * xi) next State
	double * const A, // (1 * xi) current dstate
	double const * const Xti, // (1 * xi) current state/initial conditions
	double const * const Uti, // (1 * ui) input at time ti
	PhysicsFunction const f,
	void * const storage
)
{
	size_t i;
	f(xi, ui, A, ti, Xti, Uti, storage);
	for (i = 0; i < xi; i++)
		Xt1[i] = Xti[i] + A[i] * dt;
}

void rk4_f_step
(
	size_t const xi,
	size_t const ui,
	double const dt, // time step
	double const ti, // current time
	double * const Xt1, // (1 * xi) next State
	double * const A, // (1 * xi) current dstate
	double * const B, // (1 * xi) solver storage/temp
	double * const C, // (1 * xi) solver storage/temp
	double * const D, // (1 * xi) solver storage/temp
	double * const Xtmp, // (1 * xi) solver storage/temp
	double const * const Xti, // (1 * xi) current state/initial conditions
	double const * const Uti, // (1 * ui) input at time ti
	double const * const Ut2, // (1 * ui) input at time ti + dt/2
	double const * const Ut1, // (1 * ui) input at time ti + dt
	PhysicsFunction const f,
	void * const storage
)
{
	size_t i;
	double const dt2 = dt / 2;
	double const t2 = ti + dt2;
	double const t1 = ti + dt;
	
	f(xi, ui, A, ti, Xti, Uti, storage);

	for (i = 0; i < xi; i++)
		Xtmp[i] = Xti[i] + A[i] * dt2;
	f(xi, ui, B, t2, Xtmp, Ut2, storage);

	for (i = 0; i < xi; i++)
		Xtmp[i] = Xti[i] + B[i] * dt2;
	f(xi, ui, C, t2, Xtmp, Ut2, storage);

	for (i = 0; i < xi; i++)
		Xtmp[i] = Xti[i] + C[i] * dt;
	f(xi, ui, D, t1, Xtmp, Ut1, storage);

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