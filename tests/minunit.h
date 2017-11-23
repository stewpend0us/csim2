#undef NDEBUG
#ifndef __minunit_h__
#define __minunit_h__

#include <stdio.h>
#include <time.h>
#include <ctype.h>
#include "dbg.h"

typedef char*(*test_function)(); //return 0 if the test passed otherwise return a string literal

#define mu_assert(test, message) if (!(test)) { log_err(message); return message; }

#define RUN_TESTS(...) int main(int argc, char const * const argv[])\
{\
    (void)argc;\
    test_function const tests[] = { __VA_ARGS__ };\
    char const names[] = #__VA_ARGS__ ",";\
    char const * start = names;\
    char const * end = strchr(start,',');\
    printf("%s\n", argv[0]);\
    size_t const count = sizeof(tests) / sizeof(test_function);\
    size_t fail_count = 0;\
    for (size_t i = 0; i<count; i++)\
    {\
        clock_t start_time = clock();\
		char * result = tests[i]();\
		clock_t end_time = clock();\
        double const time_spent = (double)(end_time - start_time) / CLOCKS_PER_SEC * 1000;\
        char * pass_fail;\
        if (result)\
        {\
            pass_fail = "FAIL";\
            fail_count++;\
        }\
        else\
        {\
            pass_fail = "PASS";\
            result = "";\
        }\
        printf("%4zu/%zu [%s] %6.4fms (%.*s) %s\n",\
            i+1, count, pass_fail, time_spent, (int)(end-start), start, result);\
        start = end+1;\
        end = strchr(start,',');\
        while (isspace(*start))\
            start++;\
    }\
    if (fail_count)\
        printf("%s, %zu FAILED\n", argv[0], fail_count);\
    else\
        printf("%s\n", argv[0]);\
}

#endif
