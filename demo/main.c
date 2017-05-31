#include "../csim2/solvers.h"

int main(int argc, char* argv[])
{
	double t = 3.0;
	size_t numSteps=1000;
	
	for (size_t i = 0; i < numSteps; i++)
	{
		rk4_f_step(t, t, numSteps, &t, &t, &t, &t, &t, &t, &t, numSteps, &t, &t, &t, NULL, NULL);
		euler_f_step(t, t, numSteps, &t, &t, &t, numSteps, &t, NULL, NULL);
	}
}