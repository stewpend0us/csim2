#ifndef _BLOCK_BASIC_H_
#define _BLOCK_BASIC_H_

#include "block.h"

struct second_order_system_storage
{
	FLOAT_TYPE zeta;
	FLOAT_TYPE omega_n;
	FLOAT_TYPE numerator;
};
/*
struct transfer_function_storage
{
	size_t num_size;
	size_t den_size;
	FLOAT_TYPE * numerator;
	FLOAT_TYPE * denominator;
};
*/
struct block * integrator( struct block * block, size_t num_blocks );
struct block * first_order_lag( struct block * block, size_t num_blocks, FLOAT_TYPE tau[] );
struct block * second_order_system( struct block * block, size_t num_blocks, struct second_order_system_storage storage[] );
//struct block * transferFunction( struct block * block, size_t num_blocks, struct transfer_function_storage storage[] );

#endif
