#ifndef _SECONDORDERSYSTEM_H_
#define _SECONDORDERSYSTEM_H_

#include "block.h"

struct secondOrderSystemStorage
{
	FLOAT_TYPE zeta;
	FLOAT_TYPE omega_n;
	FLOAT_TYPE numerator;
};

struct block * secondOrderSystem( struct block * block, size_t num_blocks, struct secondOrderSystemStorage storage[] );

#endif
