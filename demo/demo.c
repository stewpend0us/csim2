#include "../csim2/integrator.h"

__declspec(dllexport) struct StrictlyProperBlock getInt1()
{
	return integrator(1);
}

__declspec(dllexport) struct StrictlyProperBlock getInt2()
{
	return integrator(2);
}

__declspec(dllexport) struct StrictlyProperBlock getInt10()
{
	return integrator(10);
}