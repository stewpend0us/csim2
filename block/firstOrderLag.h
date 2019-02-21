#ifndef _FIRSTORDERLAG_H_
#define _FIRSTORDERLAG_H_

#include "block.h"

struct block * firstOrderLag( struct block * block, size_t numBlocks, FLOAT_TYPE tau[] );

#endif
