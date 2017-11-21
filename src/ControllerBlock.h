#pragma once
#include <stdlib.h>

struct ControllerBlockInfo
{
	size_t numFeedback;
	size_t numInputs;
	size_t numCommands;
	void * storage;
};

typedef void(*ControllerFunction)(
	struct ControllerBlockInfo const * const info,
	double * const command,
	double const time,
	double const * const feedback,
	double const * const input
);

struct ControllerBlock
{
	struct ControllerBlockInfo const info;
	ControllerFunction const c;
};