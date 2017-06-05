#pragma once

typedef struct StrictlyProperBlock * (*constructor)(char const * const options); // return a heap allocated StrictlyProperBlock given some string options
typedef void(*destructor)(struct StrictlyProperBlock * block); // pointer to a function to free the heap allocated object

// struct to be exported by dlls
struct dllStrictlyProperBlock
{
	constructor constructor;
	destructor destructor;
};