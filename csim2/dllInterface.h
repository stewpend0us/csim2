#pragma once
#include "../csim2/StrictlyProperBlock.h"

typedef struct StrictlyProperBlock * (*ConstructorFunction)(char const * const options); // return a heap allocated StrictlyProperBlock given some string options
typedef void(*DestructorFunction)(struct StrictlyProperBlock * block); // pointer to a function to free the heap allocated object

// struct to be exported by dlls
struct dllStrictlyProperBlock
{
	ConstructorFunction constructor;
	DestructorFunction destructor;
};

// use it like:
// __declspec(dllexport) struct dllStrictlyProperBlock <Name> = { <constructor_function>, <free_function> };
