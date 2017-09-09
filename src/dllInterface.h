#pragma once
#include "../csim2/StrictlyProperBlock.h"

#if defined(_MSC_VER)
# define EXPORT __declspec(dllexport)
# define IMPORT __declspec(dllimport)
#elif defined(__GNUC__)
# define EXPORT __attribute__((visibility("default")))
# define IMPORT
#endif

typedef struct StrictlyProperBlock * (*ConstructorFunction)(char const * const options); // return a heap allocated StrictlyProperBlock given some string options
typedef void(*DestructorFunction)(struct StrictlyProperBlock * block); // pointer to a function to free the heap allocated object

// struct to be exported by dlls
struct dllStrictlyProperBlock
{
	ConstructorFunction constructor;
	DestructorFunction destructor;
};

// use it like:
// EXPORT struct dllStrictlyProperBlock <Name> = { <constructor_function>, <free_function> };
