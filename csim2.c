
#include <stdlib.h>
#include <assert.h>
/*
#include "csim2.h"


// First order methods
double const z[1] = { 0 };
double const o[1] = { 1 };
ButcherTableau const Euler = { 1, z, o, z };

// Second order methods
double const Midpoint_a[4] = 
{
	0,		0,
	0.5,	0
};
double const Midpoint_b[2] =
{
	0,		1
};
double const Midpoint_c[2] =
{
	0,		0.5
};
ButcherTableau const Midpoint = { 2, Midpoint_a, Midpoint_b, Midpoint_c };

// Third order methods
double const RK3classic_a[9] =
{
	0,		0,	0,
	0.5,	0,	0,
	-1,		2,	0
};
double const RK3classic_b[3] =
{
	0.16666666666666666666666666666667,
	0.66666666666666666666666666666667,
	0.16666666666666666666666666666667
};
double const RK3classic_c[3] =
{
	0,
	0.5,
	1
};
ButcherTableau const RK3classic = { 3, RK3classic_a, RK3classic_b, RK3classic_c };

// Fourth order methods
double const RK4classic_a[16] =
{
	0,		0,		0,	0,
	0.5,	0,		0,	0,
	0,		0.5,	0,	0,
	0,		0,		1,	0
};
double const RK4classic_b[4] =
{
	0.16666666666666666666666666666667,
	0.33333333333333333333333333333333,
	0.33333333333333333333333333333333,
	0.16666666666666666666666666666667
};
double const RK4classic_c[4] =
{
	0,
	0.5,
	0.5,
	1
};
ButcherTableau const RK4classic = { 4, RK4classic_a, RK4classic_b, RK4classic_c };

double const RK4_3_8ths_a[16] =
{
	0,										0,	0,	0,
	0.33333333333333333333333333333333,		0,	0,	0,
	-0.33333333333333333333333333333333,	1,	0,	0,
	1,										-1,	1,	0
};
double const RK4_3_8ths_b[4] =
{
	0.125,
	0.375,
	0.375,
	0.125
};
double const RK4_3_8ths_c[4] =
{
	0,
	0.33333333333333333333333333333333,
	0.66666666666666666666666666666667,
	1
};
ButcherTableau const RK4_3_8ths = { 4, RK4_3_8ths_a, RK4_3_8ths_b, RK4_3_8ths_c };

void SolverStorage_New(SolverStorage * ss, ButcherTableau const * const solver, SimSettings const * const sim)
{
	ss->t = 0;
	size_t stateSize = sim->numStates * sizeof(double);
	size_t inputSize = sim->numInputs * sizeof(double);
	size_t stageSize = solver->order * sizeof(double);
	
	ss->dstate = (double *)malloc(stateSize);
	assert(ss->dstate != NULL);
	ss->state = (double *)calloc(1, stateSize);
	assert(ss->state != NULL);
	ss->input = (double *)calloc(1, inputSize);
	assert(ss->input != NULL);
	ss->hbk = (double *)malloc(stateSize);
	assert(ss->hbk != NULL);
	ss->yn = (double *)malloc(stateSize);
	assert(ss->yn != NULL);
	ss->k = (double *)malloc(stateSize*stageSize);
	assert(ss->k != NULL);
}

void SolverStorage_Dispose(SolverStorage * ss)
{
	if (ss->dstate != NULL)
	{
		free(ss->dstate);
	}
	if (ss->state != NULL)
	{
		free(ss->state);
	}
	if (ss->input != NULL)
	{
		free(ss->input);
	}
	if (ss->hbk != NULL)
	{
		free(ss->hbk);
	}
	if (ss->yn != NULL)
	{
		free(ss->yn);
	}
	if (ss->k != NULL)
	{
		free(ss->k);
	}
}

void RungeKutta_Initialize(double const t0, double * const initialState, SimSettings const * const sim, SolverStorage * const ss)
{
	size_t i;
	double * const state = ss->state;
	double * input = ss->input;
	void * const userData = ss->userData;
	void const * const systemConstants = ss->systemConstants;

	ss->t = t0;
	for (i = 0; i < sim->numStates; i++)
	{
		state[i] = initialState[i];
	}
	sim->Inputs(t0, state, input, userData, systemConstants);
}


void RungeKutta(ButcherTableau const * const bt, SimSettings const * const sim, SolverStorage * const ss)
{
	size_t const numStages = bt->order;
	double const * const a = bt->a;
	double const * const b = bt->b;
	double const * const c = bt->c;

	size_t const numStates = sim->numStates;
	double const h = sim->h;

	double const t = ss->t;
	//double * const dstate = ss->dstate;
	double * const state = ss->state;
	double * input = ss->input;
	double * const y_stage = ss->yn;
	double * const hbk = ss->hbk;
	double * const k = ss->k;
	void * const userData = ss->userData;
	void const * const systemConstants = ss->systemConstants;

	double t_stage;
	double const* a_stage;
	double ha_stage_pstage;
	double hb_stage;
	double * k_pstage;
	double * k_stage;

	size_t stagei, pstagei, statei;

	sim->Outputs(t, state, input, userData, systemConstants);

	for (statei = 0; statei < numStates; statei++)
	{
		hbk[statei] = 0;
	}

	for (stagei = 0; stagei < numStages; stagei++)
	{
		k_stage = k + stagei * numStages;
		t_stage = t + h * c[stagei];
		hb_stage = h * b[stagei];
		a_stage = a + stagei * numStages;

		for (statei = 0; statei < numStates; statei++)
		{
			y_stage[statei] = state[statei];
		}
		for (pstagei = 0; pstagei < stagei; pstagei++)
		{
			ha_stage_pstage = h*a_stage[pstagei];
			k_pstage = k + pstagei * numStages;
			for (statei = 0; statei < numStates; statei++)
			{
				y_stage[statei] += ha_stage_pstage * k_pstage[statei];
			}
		}

		sim->Inputs(t_stage, y_stage, input, userData, systemConstants);
		sim->dStates(t_stage, k_stage, y_stage, input, systemConstants);

		for (statei = 0; statei < numStates; statei++)
		{
			hbk[statei] += hb_stage * k_stage[statei];
		}
	}

	ss->t += h;
	for (statei = 0; statei < numStates; statei++)
	{
		state[statei] += hbk[statei];
	}
}

*/