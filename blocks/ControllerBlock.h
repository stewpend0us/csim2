#pragma once
#include <stdlib.h>

struct ControllerBlock;

typedef void(*ControllerFunction)(
	struct ControllerBlock const * const block,
	double * const command,
	double const time,
	double const * const feedback,
	double const * const input
);

struct ControllerBlock
{
	size_t numFeedback;
	size_t numInputs;
	size_t numCommands;
	void * storage;
	ControllerFunction c;
};