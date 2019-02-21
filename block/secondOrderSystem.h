#ifndef _SECONDORDERSYSTEM_H_
#define _SECONDORDERSYSTEM_H_

#include "block.h"

struct secondOrderSystemStorage
{
	FLOAT_TYPE zeta;
	FLOAT_TYPE omega_n;
};

struct block * secondOrderSystem( struct block * block, size_t numBlocks, struct secondOrderSystemStorage storage[] );
FLOAT_TYPE * secondOrderSystemOutput( FLOAT_TYPE output[], size_t numOutputs, FLOAT_TYPE const state[] );
