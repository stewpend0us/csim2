#pragma once
//inspired by http://www.jera.com/techinfo/jtns/jtn002.html

#include <stddef.h>

#define assert_is_true(test, message) do { if (!(test)) return message; } while (0)

typedef char*(*test_function)(); //return 0 if the test passed otherwise return a string literal

void run_tests(size_t count, test_function*functions);

void run_all_tests();