#ifndef _TEST_H_
#define _TEST_H_

#include <stdio.h>
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define AT __FILE__ ":" TOSTRING(__LINE__) ": "
#define DEBUG( ... ) { printf( AT "\033[1;41m" __VA_ARGS__); printf("\033[0m\n"); }
#define ASSERT( T, ... ) if ( !(T) ) { DEBUG( __VA_ARGS__ ); exit(__LINE__); }
#define PASS printf( __FILE__ ": \033[1;42mpassed\033[0m\n" )

#endif
