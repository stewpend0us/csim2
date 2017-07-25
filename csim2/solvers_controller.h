#pragma once
#include "solvers.h"

typedef void(*ControllerFunction)(
	double const time,
	size_t const numCommand,
	double const * const command,
	size_t const numFeedback,
	double const * const feedback,
	size_t const numOutput,
	double * const output,
	void * const storage
	);